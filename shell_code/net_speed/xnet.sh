#! /bin/bash

function main()
{
	netdev=$(cat /proc/net/dev | sed -n '3,$ p' | awk -F ":" '{print $1}' | sed '/lo/d' | sed 's/ //g')
	netdev=$(ifconfig|grep eth|awk '{print $1}')
	
	for devname in $netdev
	do
		./show_speed.sh $devname& 
	done
}

main

