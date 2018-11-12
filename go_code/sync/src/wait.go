package main

import (
	"fmt"
	"sync"
)

func main() {
	var wg sync.WaitGroup

	i := 0
	for ; i < 10; i++ {
		wg.Add(1)
		go func(i int) {
			fmt.Println("Hello world test: ", i)
			wg.Done()
		}(i)
	}

	wg.Wait()

	fmt.Println("run done: ", i)
}

