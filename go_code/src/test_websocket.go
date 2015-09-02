package main

import (
	"fmt"
	"golang.org/x/net/websocket"
	"html/template"
	"io"
	"net/http"
	"os"
)

var (
	pwd, _   = os.Getwd()
	RootTemp = template.Must(template.ParseFiles(pwd + "/file.html"))
	JSON     = websocket.JSON    // codec for JSON
	Message  = websocket.Message // codec for string, []byte
)

// Echo the data received on the WebSocket.
func EchoServer(ws *websocket.Conn) {
	io.Copy(ws, ws)
}

func TestServer(c http.ResponseWriter, req *http.Request) {
	err := RootTemp.Execute(c, req.Host)
	if err != nil {
		http.Error(c, err.Error(), http.StatusInternalServerError)
	}
}

func Echo(ws *websocket.Conn) {
	var err error

	for {

		var reply string

		if err = websocket.Message.Receive(ws, &reply); err != nil {
			fmt.Println("Can't receive")
			break
		}

		fmt.Println("Received back from client: " + reply)

		msg := "Received:  " + reply
		fmt.Println("Sending to client: " + msg)

		if err = websocket.Message.Send(ws, msg); err != nil {
			fmt.Println("Can't send")
			break
		}
	}
}

// This example demonstrates a trivial echo server.
func main() {
	http.HandleFunc("/test", TestServer)
	http.Handle("/", http.FileServer(http.Dir(".")))
	http.Handle("/echo", websocket.Handler(Echo))
	http.Handle("/echo2", websocket.Handler(EchoServer))
	err := http.ListenAndServe(":12345", nil)
	if err != nil {
		panic("ListenAndServe: " + err.Error())
	}
}
