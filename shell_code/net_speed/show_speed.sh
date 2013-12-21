#! /bin/bash

typeset in_old out_old speed_type

function print_dev()
{
	sleep 1
	in_new=$(cat /proc/net/dev | grep $1 | sed 's=^.*:==' | awk '{ print $1 }')
	out_new=$(cat /proc/net/dev | grep $1 | sed 's=^.*:==' | awk '{ print $9 }')
	in_tmp=$((${in_new} - ${in_old}))
	out_tmp=$((${out_new} - ${out_old}))
	if [ $in_tmp -gt 1000000000 ]; then
		in_tmp1=$(($in_tmp / 1024 / 1024 / 1024))
		out_tmp1=$(($out_tmp / 1024 / 1024 / 1024))
		speed_type="GB/s"
	elif [ $in_tmp -gt 1000000 ]; then
		in_tmp1=$(($in_tmp / 1024 /1024 ))
		out_tmp1=$(($out_tmp / 1024 / 1024))
		speed_type="MB/s"
	elif [ $in_tmp -gt 1024 ]; then
		in_tmp1=$(($in_tmp / 1024))
		out_tmp1=$(($out_tmp / 1024))
		speed_type="KB/s"
	else
		in_tmp1=$in_tmp
		out_tmp1=$out_tmp
		speed_type="bytes"
	fi
	in_old=$in_new
	out_old=$out_new
	
	echo -ne "\r$1--- IN: ${in_tmp1} ${speed_type} OUT: ${out_tmp1} ${speed_type}        "
}

function main()
{
	
	in_old=$(cat /proc/net/dev | grep $1 | sed 's=^.*:==' | awk '{ print $1 }')
	out_old=$(cat /proc/net/dev | grep $1 | sed 's=^.*:==' | awk '{ print $9 }')
	while true
	do
		print_dev $1;
	done
}

main $1

