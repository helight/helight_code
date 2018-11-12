package main

import (
	"bytes"
	"fmt"
	"time"

	kcp "github.com/xtaci/kcp-go"
)

func main() {
	conn, err := kcp.Dial("0.0.0.0:6666")
	if err != nil {
		panic(err)
		return
	}
	for {
		ret, err := conn.Write([]byte("hello kcp!!!"))
		if err != nil {
			fmt.Println(err)
			return
		}
		fmt.Println(ret)
		datas := bytes.NewBuffer(nil)
		var buf[512] byte
		n, err := conn.Read(buf[0:])
		datas.Write(buf[0:n])
		fmt.Println("read: ", string(datas.Bytes()))
		time.Sleep(1 * time.Second)
	}
}
