package main

import (
	"fmt"
	"net"
	"time"

	kcp "github.com/xtaci/kcp-go"
)

func main() {
	listen, err := kcp.Listen("0.0.0.0:6666")
	if err != nil {
		panic(err)
		return
	}
	for {
		conn, err := listen.Accept()
		if err != nil {
			panic(err)
			return
		}

		go handleconn(conn)
	}
}

func handleconn(conn net.Conn) {
	fmt.Println("new client", conn.RemoteAddr())

	// udpsession := conn.(UDPSession)
	// fmt.Println("new client", conn.GetConv())
	buf := make([]byte, 65536)
	count := 0
	for {
		conn.SetReadDeadline(time.Now().Add(10 * time.Second))
		n, err := conn.Read(buf)
		fmt.Println("new client read: ", n, " ", string(buf))
		if err != nil {
			fmt.Println("new client read err", n)
			conn.Close()
			return
		}
		count++
		conn.Write(buf[:n])
	}
}
