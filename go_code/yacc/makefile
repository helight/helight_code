
all:
	go tool yacc -o calc.go -p Calc calc.y
	go build -o cal *.go
clean:
	go clean
	rm *.go y.output cal