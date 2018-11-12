#!/bin/bash

printf "\033[1;33m"
echo "Begin to build ..."
printf "\033[m"

cd ..

export GOPATH=$GOPATH:`pwd`
echo $GOPATH

cd -
go build tcpserver.go
go build tcpclient.go
go build udpserver.go
go build udpclient.go
go build kcpserver.go
go build kcpclient.go
