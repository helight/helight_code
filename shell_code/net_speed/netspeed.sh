#!/bin/bash

if [ $# -lt 1 ];then 
	echo "Parame not enough!!"
	exit 1
fi

devinfo=devinfo.txt
rxtx=rxtx.txt
INTERVAL=$1

while true
do
	echo -ne "\r"
	ifconfig > $devinfo
	cat $devinfo | grep "RX bytes" | awk -F: '{print $2}' | awk '{print $1}' > $rxtx
	COUNT=0
	while read RX1[$[COUNT]]; do
	    COUNT=$[COUNT+1]
	done < $rxtx
	cat $devinfo | grep "TX bytes" | awk -F: '{print $3}' | awk '{print $1}' > $rxtx
	COUNT=0
	while read TX1[$[COUNT]]; do
	    COUNT=$[COUNT+1]
	done < $rxtx

	sleep $INTERVAL

	ifconfig > $devinfo
	cat $devinfo | grep "RX bytes" | awk -F: '{print $2}' | awk '{print $1}' > $rxtx
	COUNT=0
	while read RX2[$[COUNT]]; do
	    COUNT=$[COUNT+1]
	done < $rxtx
	cat $devinfo | grep "TX bytes" | awk -F: '{print $3}' | awk '{print $1}' > $rxtx
	COUNT=0
	while read TX2[$[COUNT]]; do
	    COUNT=$[COUNT+1]
	done < $rxtx

	cat $devinfo | grep "Link encap" | awk '{print $1}' > $rxtx
	COUNT=0
	while read IF[$[COUNT]]; do
	    RX=`expr ${RX2[$[COUNT]]} - ${RX1[$[COUNT]]}`
	    TX=`expr ${TX2[$[COUNT]]} - ${TX1[$[COUNT]]}`
	    R0=$[$RX/$INTERVAL/1024]
	    T0=$[$TX/$INTERVAL/1024]
	    R1=$[$RX/$INTERVAL/102-$R0*10]
	    T1=$[$TX/$INTERVAL/102-$T0*10]
	    echo -n "${IF[$[COUNT]]}  RX: ${R0}.${R1}KB/s  TX: ${T0}.${T1}KB/s        "
	    COUNT=$[COUNT+1]
	done < $rxtx

	rm -f $devinfo $rxtx
done
