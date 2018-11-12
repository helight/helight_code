package org.zhwen.test;

import org.apache.thrift.TProcessor;
import org.apache.thrift.protocol.TCompactProtocol;
import org.apache.thrift.server.TNonblockingServer;
import org.apache.thrift.server.TServer;
import org.apache.thrift.transport.TFramedTransport;
import org.apache.thrift.transport.TNonblockingServerSocket;

 

public class HelloServerDemo {
	public static final int SERVER_PORT = 8090;
 
	public void startServer() {
		try {
			System.out.println("HelloWorld NonblockingServer start ...."); 
		
			TProcessor tprocessor = new HelloWorldService.Processor<HelloWorldService.Iface>(
					new HelloWorldImpl());
 
			TNonblockingServerSocket tnbSocketTransport = new TNonblockingServerSocket(SERVER_PORT);
			TNonblockingServer.Args tnbArgs = new TNonblockingServer.Args(tnbSocketTransport);
			tnbArgs.processor(tprocessor);
			tnbArgs.transportFactory(new TFramedTransport.Factory());
			tnbArgs.protocolFactory(new TCompactProtocol.Factory());
 
			// 使用非阻塞式IO，服务端和客户端需要指定TFramedTransport数据传输的方式
			TServer server = new TNonblockingServer(tnbArgs);
			server.serve();
 
		} catch (Exception e) {
			System.out.println("Server start error!!!");
			e.printStackTrace();
		}
	}
 
	/**
	 * @param args
	 */
	public static void main(String[] args) {
		HelloServerDemo server = new HelloServerDemo();
		server.startServer();
	}
 
}
