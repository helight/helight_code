#!/bin/bash

printf "\033[1;33m"
echo "Begin to build ..."
printf "\033[m"

cd ..

export GOPATH=$GOPATH:`pwd`
echo $GOPATH

cd -
go build wait.go
go build signal.go