#!/bin/sh

# create parser
for y in *.y
do
    go tool yacc -o ${y%.y}.go -p Calc $y
done

# go tool yacc -o calc.go -p Calc calc.y
# build go
go build