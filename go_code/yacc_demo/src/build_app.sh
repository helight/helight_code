#!/bin/sh
# build app
# writed by helightxu on 2015/05/14

printf "\033[1;33m"
echo "Hi:"
echo "  Begin to build ... ..."
printf "\033[m"

cd ..
export GOPATH=`pwd`:`pwd`/../thirdparty/
echo $GOPATH

cd -

function build_all()
{
    echo "Begin to build......"

    go tool yacc -o calc.go -p Calc calc.y
    go build calc.go

    echo "build finished"
}

build_all
