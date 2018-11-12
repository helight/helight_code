#!/usr/bin/env python
#coding=utf-8

import httplib, urllib2, sys, socket
import json, time
import thread, threading

class CrawlerSpider(threading.Thread):
	def __init__(self):
		pass
	def run(self):
		pass
	def __del__(self):
		pass

def crawler_url(str_url):
	try:
		fd = urllib2.urlopen(str_url, timeout = 20)
		status = fd.getcode()
		info = fd.info()
		content = fd.read()
		fd = open("test.txt", "w")
		fd.write(content)
		fd.close()
		print "url: " + str_url.strip() + " status: " + str(status) + " content_length:" + str(len(content))
	except urllib2.HTTPError, e:  #错误状态码
		print "url: " + str_url.strip() + " error: " + str(e.code) #+ reason
	except urllib2.URLError, e:   #超时
		print "url: " + str_url.strip() + " error: timeout" #+ e.reason
	except:
		print "url: " + str_url.strip() + " error: other"  
	pass

def read_url_file(str_file_name):
	fd = open(str_file_name,"r")
	str_line = fd.readline()
	while str_line:
		print str_line
		crawler_url(str_line)
		str_line = fd.readline()
	fd.close()

if __name__ == "__main__":
	read_url_file("200.txt")
