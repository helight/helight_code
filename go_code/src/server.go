package main

import (
	"code.google.com/p/go.crypto/ssh"
	"encoding/json"
	"flag"
	"fmt"
	//	_ "github.com/go-sql-driver/mysql"
	"golang.org/x/net/websocket"
	"io"
	"log"
	"net/http"
	"strconv"
	"strings"
)

type Writer struct {
	io.Writer
	Conn *websocket.Conn
}

type WsResponse struct {
	Command    string                 `json:"cmd"`
	Parameters map[string]interface{} `json:"params"`
}

func (o *Writer) Write(p []byte) (n int, err error) {
	resp := WsResponse{Command: "output", Parameters: map[string]interface{}{"content": string(p)}}
	json_bytes, err := json.Marshal(resp)
	if err != nil {
		fmt.Println("Marshal error:", err)
		return
	}
	websocket.Message.Send(o.Conn, string(json_bytes))
	return len(p), nil
}

type ptyWindowChangeMsg struct {
	Columns uint32
	Rows    uint32
	Width   uint32
	Height  uint32
}

type ptyEnvMsg struct {
	Name  string
	Value string
}

func Echo(ws *websocket.Conn) {
	ip := ws.Request().URL.Query().Get("ip")
	fmt.Println("conn ip", ip)

	port := 22
	username := "ding"
	password := "ding"

	authMethods := []ssh.AuthMethod{}
	keyboardInteractiveChallenge := func(
		user,
		instruction string,
		questions []string,
		echos []bool,
	) (answers []string, err error) {
		if len(questions) == 0 {
			return []string{}, nil
		}
		return []string{password}, nil
	}
	authMethods = append(authMethods, ssh.KeyboardInteractive(keyboardInteractiveChallenge))
	authMethods = append(authMethods, ssh.Password(password))
	sshConfig := &ssh.ClientConfig{
		User: username,
		Auth: authMethods,
	}
	sshConfig.Config.SetDefaults()
	sshConfig.Config.Ciphers = append(sshConfig.Config.Ciphers, "arcfour")
	fmt.Println(sshConfig.Config.Ciphers)

	conn, err := ssh.Dial("tcp", fmt.Sprintf("%s:%v", ip, port), sshConfig)
	if err != nil {
		fmt.Println("1Unable to connect %s", err)
		return
	}
	defer conn.Close()
	cmd, err := conn.NewSession()
	if err != nil {
		fmt.Println("Can't NewSession", err)
		return
	}
	modes := ssh.TerminalModes{
		ssh.ECHO:          1,     // disable echoing
		ssh.TTY_OP_ISPEED: 14400, // input speed = 14.4kbaud
		ssh.TTY_OP_OSPEED: 14400, // output speed = 14.4kbaud
	}
	// Request pseudo terminal
	if err := cmd.RequestPty("xterm", 80, 40, modes); err != nil {
		fmt.Println("request for pseudo terminal failed: %s", err)
	}

	cmd.Stdout = &Writer{Conn: ws}
	cmd.Stderr = &Writer{Conn: ws}
	stdin, err := cmd.StdinPipe()
	if err != nil {
		fmt.Println("Can't get stdin")
		return
	}

	err = cmd.Shell()
	if err != nil {
		fmt.Println("Can't start cmd:", err)
		return
	}

	for {
		var reply string

		if err = websocket.Message.Receive(ws, &reply); err != nil {
			fmt.Println("Can't receive")
			break
		}

		fmt.Println("Received back from client: " + reply)

		request := make(map[string]interface{}, 0)
		err := json.Unmarshal([]byte(reply), &request)
		fmt.Println("Received back from client: ", request)
		if err == nil {
			switch request["cmd"].(string) {
			case "window-change":
				w, _ := strconv.Atoi(fmt.Sprint(request["width"]))
				h, _ := strconv.Atoi(fmt.Sprint(request["height"]))
				if h > 0 && w > 0 {
					req := ptyWindowChangeMsg{
						Columns: uint32(w),
						Rows:    uint32(h),
						Width:   uint32(w * 8),
						Height:  uint32(h * 8),
					}
					fmt.Println("Send window-change:", req)
					ok, err := cmd.SendRequest("window-change", true, ssh.Marshal(&req))
					// Request pseudo terminal
					if err == nil && !ok {
						fmt.Println("request for pseudo terminal failed: %v", err)
					}
				}
			case "shell":
				fmt.Println("Write:", request["input"])
				stdin.Write([]byte(request["input"].(string)))

			case "ping":
				req := ptyEnvMsg{Name: "__keep_alive__", Value: "1"}
				cmd.SendRequest("env", true, ssh.Marshal(&req))

				resp := WsResponse{Command: "pong", Parameters: map[string]interface{}{}}
				json_bytes, err := json.Marshal(resp)
				if err != nil {
					fmt.Println("Marshal error:", err)
					return
				}
				websocket.Message.Send(ws, string(json_bytes))
			}
		} else {
			fmt.Println(err)
		}
	}
}

func main() {
	fs := http.FileServer(http.Dir("static"))
	http.Handle("/static/", http.StripPrefix("/static/", fs))
	http.HandleFunc("/", func(w http.ResponseWriter, r *http.Request) {
		if r.URL.Path == "/" {
			w.Write([]byte(strings.Replace(terminal_html, "{{ip}}", r.URL.Query().Get("ip"), -1)))
		} else {
			fmt.Println("Not handle:", r.URL)
		}
	})
	http.Handle("/ws", websocket.Handler(Echo))
	if err := http.ListenAndServe(":9999", nil); err != nil {
		log.Fatal("ListenAndServe:", err)
	}
}

func init() {
	if !flag.Parsed() {
		flag.Parse()
	}
}

var terminal_html = `
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xmlns:v="urn:schemas-microsoft-com:vml" xml:lang="en" lang="en">
  <head>
    <title>Shell In A Box</title>

    <link rel="stylesheet" href="static/styles.css" type="text/css" />
    <style type="text/css">
      body {
        margin: 0px;
      }
      html, body{ margin:0; height:100%; }
    </style>
    <script type="text/javascript"><!--
      (function() {
        if (typeof navigator.appName == 'undefined' ||
            navigator.appName != 'Netscape') {
          document.write('<style type="text/css">' +
                         '#vt100 #console div, #vt100 #alt_console div {' +
                         '  overflow: hidden;' +
                         '}' +
                         '</style>');
        }
      })();

    --></script>
    <link rel="shortcut icon" href="favicon.ico" type="image/x-icon" />
    <script type="text/javascript" src="static/vt100.js"></script>
    <script type="text/javascript" src="static/shell_in_a_box.js"></script>
    <link rel="stylesheet" href="static/pro.css" type="text/css" />
  </head>


  <body onload=""
        scroll="no"><noscript>JavaScript
    must be enabled for ShellInABox</noscript>
<script type="text/javascript">
setTimeout('new ShellInABox(\'ws://\' + window.location.host+window.location.pathname + \'ws?ip={{ip}}\')', 1);
</script>
    </body>
</html>
`
