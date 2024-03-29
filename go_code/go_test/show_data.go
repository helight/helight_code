// show_data.go
package main

import (
	"fmt"
	"math"
)

func show_data1() {
	fmt.Println("test")
}

func add_num1(a int, b int) int {
	return a + b
}

func fib1() func() int {
	a, b := 0, 1
	return func() int {
		a, b = b, a+b
		return a
	}
}

// pi launches n goroutines to compute an
// approximation of pi.
func pi1(n int) float64 {
	ch := make(chan float64)
	for k := 0; k <= n; k++ {
		go term1(ch, float64(k))
	}
	f := 0.0
	for k := 0; k <= n; k++ {
		f += <-ch
	}
	return f
}

func term1(ch chan float64, k float64) {
	ch <- 4 * math.Pow(-1, k) / (2*k + 1)
}

/*func main() {
	fmt.Println("Hello World!")
}*/
