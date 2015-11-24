package server;

import org.apache.thrift.TProcessorFactory;
import org.apache.thrift.protocol.TCompactProtocol;
import org.apache.thrift.server.TNonblockingServer;
import org.apache.thrift.server.TServer;
import org.apache.thrift.transport.TFramedTransport;
import org.apache.thrift.transport.TNonblockingServerSocket;
//import org.apache.thrift.server.TThreadPoolServer;

// Generated code
import rpc.tutorial.*;
import rpc.shared.*;

public class JavaServer {
	private static int port = 9090;
	private void start() {
		try {
			CalculatorHandler handler = new CalculatorHandler();
			final Calculator.Processor processor = new Calculator.Processor(handler);

			Runnable nonblockserver = new Runnable() {
				public void run() {
					try {
						TNonblockingServerSocket tServerSocket =                                                      
				        new TNonblockingServerSocket(
				        		new TNonblockingServerSocket.NonblockingAbstractServerSocketArgs().port(port));
						
						TNonblockingServer.Args args = new TNonblockingServer.Args(tServerSocket);
						args.protocolFactory(new TCompactProtocol.Factory());  
			            args.transportFactory(new TFramedTransport.Factory());  
			            args.processorFactory(new TProcessorFactory(processor));  
						TServer server = new TNonblockingServer(args);						

						// TServerTransport serverTransport = new TServerSocket(9090);
						// TServer server = new TSimpleServer(new Args(serverTransport).processor(processor));

						// Use this for a multithreaded server
						// TServer server = new TThreadPoolServer(new
						// TThreadPoolServer.Args(serverTransport).processor(processor));

						System.out.println("Starting the nonblock server...");
						server.serve();
					} catch (Exception e) {
						e.printStackTrace();
					}
				}
			};

			new Thread(nonblockserver).start();

		} catch (Exception x) {
			x.printStackTrace();
		}
	}
	
	public static void main(String[] args) {
		JavaServer svr = new JavaServer();
		svr.start();		
	}
}