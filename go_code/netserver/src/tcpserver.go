package main

import (
	"fmt"
	"net"
	"os"
)

var (
	count = 0
	max_pool_num = 5
	task_channel = make(chan net.Conn)
)

func main() {
	ihost := ":5555"
	tcpAddr, err := net.ResolveTCPAddr("tcp4", ihost)
	if err != nil {
		fmt.Printf("Fatal error: ", err)
		os.Exit(1)
	}
	listener, err := net.ListenTCP("tcp", tcpAddr)
	if err != nil {
		fmt.Printf("Fatal error: ", err)
		os.Exit(1)
	}
	defer listener.Close()
	go init_process_pool()

	for {
		conn, err := listener.Accept()
		if err != nil {
			continue
		}
		count += 1
		// rep := fmt.Sprintf("hello %d", count)
		// conn.Write([]byte(rep))
		// conn.Close()
		// go handleConn(conn)
		task_channel <- conn
	}
}

func init_process_pool(){
	for i := 0; i < max_pool_num; i++ {
		go func() {
		for {
			select {
			case conn := <- task_channel:
				handleConn(conn)
			}
		}
		}()
	}
}
func handleConn(c net.Conn) {
	fmt.Println("begin to do tcp request")
	buffer := make([]byte, 1024)
	c.Read(buffer)
	rep := fmt.Sprintf("get: %s return: hell %d", buffer, count)

	c.Write([]byte(rep))
	c.Close()
}
