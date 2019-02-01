// datacal.go

package main

import "testing"

func Test_add_num(t *testing.T) {
	type args struct {
		a int
		b int
	}
	tests := []struct {
		name string
		args args
		want int
	}{
		// TODO: Add test cases.
		{"1 + 1", args{1, 1}, 2},
		{"1 + 2", args{1, 2}, 3},
		{"1 + 3", args{1, 3}, 4},
	}

	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			if got := add_num(tt.args.a, tt.args.b); got != tt.want {
				t.Errorf("add_num() = %v, want %v", got, tt.want)
			}
		})
	}
}

func Benchmark_add_num(b *testing.B) {
	sum := 0
	for i := 0; i < b.N; i++ {
        sum = add_num(1343, 434)
	}
	b.Log(sum)
}
