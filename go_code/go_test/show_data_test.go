// show_data_test.go
package main

import (
	"fmt"
	"testing"
)

func Test_show_data1(t *testing.T) {

	show_data1()
	show_data1()
}

func Test_fib1(t *testing.T) {
	f := fib1()
	fmt.Println(f(), f(), f(), f(), f())
}

func Test_pi1(t *testing.T) {
	fmt.Println(pi1(5000))
	// t.Errorf("tfasdfsd")

}

func Test_add_num1(t *testing.T) {
	var tests = []struct {
		a   int
		b   int
		out int
	}{
		{1, 2, 3},
		{3, 4, 7},
		{4, 6, 10},
		{2, 2, 4},
		{3, 3, 6},
	}
	// addi := add_num(1, 4)
	for _, itest := range tests {
		if itest.out != add_num1(itest.a, itest.b) {
			t.Errorf("add num error:%d + %d = %d", itest.a, itest.b, itest.out)
		}
	}

}

func Benchmark_fib1(b *testing.B) {
	for i := 0; i < b.N; i++ {
		pi1(500)
	}
}
