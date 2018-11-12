package main

import (
	"fmt"
	"os"
	"os/signal"
	"syscall"
	"time"
)

func main() {
	sigs := make(chan os.Signal, 1)
	// done := make(chan bool, 1)

	// signal.Notify(sigs)
	// signal.Notify(sigs, os.Interrupt, os.Kill, syscall.SIGUSR1, syscall.SIGUSR2, syscall.SIGINT, syscall.SIGTERM)
	signal.Notify(sigs, syscall.SIGUSR1, syscall.SIGUSR2, syscall.SIGINT, syscall.SIGTERM, syscall.SIGHUP, syscall.SIGQUIT)

	// go func() {
	// 	sig := <-sigs
	// 	fmt.Println(sig)
	// 	done <- true
	// }()
	go func() {
		for s := range sigs {
			switch s {
			case syscall.SIGINT, syscall.SIGTERM, syscall.SIGHUP, syscall.SIGQUIT:
				fmt.Println("got signal and try to exit: ", s)
				do_exit()
			case syscall.SIGUSR1:
				fmt.Println("usr1: ", s)
			case syscall.SIGUSR2:
				fmt.Println("usr2: ", s)
			default:
				fmt.Println("other: ", s)
			}
		}
	}()


	fmt.Println("wait for signal")
	i := 0
	for {
		i++
		fmt.Println("times: ", i)
		time.Sleep(1 * time.Second)
	}
	// <- done
	fmt.Println("got signal and exit")
	fmt.Println("run done")
}
 
func do_exit() {
	fmt.Println("try do some clear jobs")
	fmt.Println("run done")
	os.Exit(0)
}
