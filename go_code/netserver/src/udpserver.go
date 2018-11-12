package main

import (
	"fmt"
	"net"
	"os"
)

var (
	count = 0
)

func main() {
	ihost := ":5555"
	udpAddr, err := net.ResolveUDPAddr("udp4", ihost)
	if err != nil {
		fmt.Printf("Fatal error: ", err)
		os.Exit(1)
	}
	listener, err := net.ListenUDP("udp", udpAddr)
	if err != nil {
		fmt.Printf("Fatal error: ", err)
		os.Exit(1)
	}
	defer listener.Close()

	for {

		count += 1
		// rep := fmt.Sprintf("hello %d", count)
		// conn.Write([]byte(rep))
		// conn.Close()
		handleConn(listener)
	}
}

func handleConn(c *net.UDPConn) {
	fmt.Println("begin to do udp request")
	buffer := make([]byte, 128)
	_, addr, err := c.ReadFromUDP(buffer[0:])
	if err != nil {
		return
	}
	rep := fmt.Sprintf("get: %s return: hell %d", buffer, count)

	_, err = c.WriteToUDP([]byte(rep), addr)
	if err != nil {
		return
		fmt.Println("send data fail: ", err)
	}
}
