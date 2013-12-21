#!/usr/bin/python

import socket
import time
import threading
from threading import Thread
import Queue

RETRY_ITV = 1 
MAX_TO = 3
MAX_RETRY = 3

def scan_tcp(ip, port):
	s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
	s.settimeout(MAX_TO)
	retry = 0
	while(retry < MAX_RETRY):
		try:
			s.connect((ip,port))
		except:
			retry += 1
			if (retry == MAX_RETRY):
				#print "[info]%s:%d ==> dead ==> alert to helight" %( ip , int(port))
				break
			else:
#				time.sleep(RETRY_ITV)
				continue
		else:
			print "[info]%s:%d ==> alive" %( ip , int(port) )
			s.close()
			break

class ip_scan(Thread):
	def __init__ (self,ip):
		Thread.__init__(self)
		self.ip = ip
	def run(self):
		for port in range(1, 65535):
			scan_tcp(self.ip, port)
#			scan_udp(ip, port)
		print ip + "  " + str(port) 
#		Thread.exit()
		
if __name__ == '__main__':
	#ip_scan("172.24.28.192")
	scanlist = []
	fd = open('iplist', 'r')
	count = 0
	for line in fd.readlines():
		ip = line.split()[0]
		current = ip_scan(ip)
		scanlist.append(current)
		current.start()
		count += 1
		if (count%5 == 0):
			time.sleep(10)
	print count
