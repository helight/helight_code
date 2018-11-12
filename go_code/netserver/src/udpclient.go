package main

import (
	"fmt"
	"net"
	"os"
)

func main() {
	if len(os.Args) != 2 {
		fmt.Fprintf(os.Stderr, "Usage: %s host:port", os.Args)
		os.Exit(1)
	}
	ihost := os.Args[1]
	udpAddr, err := net.ResolveUDPAddr("udp4", ihost)
	if err != nil {
		fmt.Fprintf(os.Stderr, "Fatal error 1: %s", err)
		os.Exit(1)
	}
	conn, err := net.DialUDP("udp", nil, udpAddr)
	if err != nil {
		fmt.Fprintf(os.Stderr, "Fatal error 2: %s", err)
		os.Exit(1)
	}
	defer conn.Close()
	_, err = conn.Write([]byte("hello server"))
	if err != nil {
		fmt.Fprintf(os.Stderr, "Fatal error: %s", err)
		os.Exit(1)
	}
	data := make([]byte, 1024)
	read, addr, err := conn.ReadFromUDP(data)
	if err != nil {
		fmt.Fprintf(os.Stderr, "Fatal error: %s", err)
		os.Exit(1)
	}
	fmt.Println("read: ", read, " add:", addr)
	fmt.Println("read data: ", string(data))
	os.Exit(0)
}
