#-*- encoding: utf-8 -*-
import string, threading, time
import socket

def thread_main(a):
	global count, mutex
#获得线程名
	threadname = threading.currentThread().getName()
	sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)  
	sock.connect(('localhost', 8088)) 

	while (1) :
# 取得锁
		mutex.acquire()
		count = count + 1
		sock.send(threadname + str(count))
		print sock.recv(1024)
# 释放锁
		mutex.release()
#print threadname, count
		time.sleep(1)
	sock.close()

class thread_test(threading.Thread):
	def __init__(self, num):
		threading.Thread.__init__(self)
		self.run_num = num
	def run(self):
		global count, mutex
		threadname = threading.currentThread().getName()
		for x in xrange(0, int(self.run_num)):
			mutex.acquire()
			count += 1
			mutex.release()
			print threadname, count
			time.sleep(1)

if __name__ == '__main__':
	global count, mutex
	threads = []
	num = 1 

	count = 1
# 创建一个锁
	mutex = threading.Lock()
# 先创建线程对象
	for x in xrange(0, num):
		threads.append(thread_test(1000))
# 启动所有线程
	for t in threads:
		t.start()
# 主线程中等待所有子线程退出
	for t in threads:
		t.join()  

