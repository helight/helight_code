#!/bin/sh
# Copyright (c) 2014, HelightXu
# Author: Zhwen Xu<HelightXu@gmail.com>
# Created: 2014-12-22
# Description:
#

libpath="."
jarlibs=`ls libs`
for item in $jarlibs
do
    libpath=$libpath":../libs/"$item
done

echo $libpath

echo $1
cd bin
if [ "$1" == "1" ] ; then
    java -classpath $libpath server.JavaServer
elif [ "$1" == "2" ] ; then
    java -classpath $libpath client.JavaClient simple
else
    echo "wrong args"
fi

