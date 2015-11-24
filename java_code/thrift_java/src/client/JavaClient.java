package client;

import java.io.IOException;

import rpc.tutorial.*;
import rpc.shared.*;

import org.apache.thrift.TException;
import org.apache.thrift.transport.TFramedTransport;
import org.apache.thrift.transport.TNonblockingSocket;
import org.apache.thrift.transport.TNonblockingTransport;
import org.apache.thrift.transport.TTransport;
import org.apache.thrift.transport.TSocket;
import org.apache.thrift.async.TAsyncClientManager;
import org.apache.thrift.protocol.TCompactProtocol;
import org.apache.thrift.protocol.TProtocol;
import org.apache.thrift.protocol.TProtocolFactory;

public class JavaClient {

	public static final String address = "127.0.0.1";
	public static final int port = 9090;
	public static final int clientTimeout = 30000;

	public void start_sync() {
		try {
			TTransport transport = new TFramedTransport(new TSocket(address,
					port, clientTimeout));
			TProtocol protocol = new TCompactProtocol(transport);
			Calculator.Client client = new Calculator.Client(protocol);

			transport.open();

			perform(client);

			transport.close();
		} catch (TException x) {
			x.printStackTrace();
		}
	}
	
	private void perform(Calculator.Client client) throws TException {
		client.ping();
		System.out.println("ping()");

		int sum = client.add(1, 1);
		System.out.println("1+1=" + sum);

		Work work = new Work();

		work.op = Operation.DIVIDE;
		work.num1 = 1;
		work.num2 = 0;
		try {
			int quotient = client.calculate(1, work);
			System.out.println("Whoa we can divide by 0");
		} catch (InvalidOperation io) {
			System.out.println("Invalid operation: " + io.why);
		}

		work.op = Operation.SUBTRACT;
		work.num1 = 15;
		work.num2 = 10;
		try {
			int diff = client.calculate(1, work);
			System.out.println("15-10=" + diff);
		} catch (InvalidOperation io) {
			System.out.println("Invalid operation: " + io.why);
		}

		SharedStruct log = client.getStruct(1);
		System.out.println("Check log: " + log.value);
	}

	public void start_async() throws Exception {
		try {
			TAsyncClientManager clientManager = new TAsyncClientManager();
			TNonblockingTransport transport = new TNonblockingSocket(address, port, clientTimeout);
			TProtocolFactory protocol = new TCompactProtocol.Factory();
			Calculator.AsyncClient asyncClient = new Calculator.AsyncClient(protocol, clientManager, transport);

			System.out.println("Client calls .....");
			AddCallback callBack = new AddCallback();
			asyncClient.add(52342, 121232, callBack);

			while (true) {
				Thread.sleep(1);
			}
		} catch (IOException e) {
			e.printStackTrace();
		}
	}

	
	public static void main(String[] args)  throws Exception{
		if (args.length != 1) {
			System.out.println("Please enter 'simple' or 'secure'");
			System.exit(0);
		}
		JavaClient client = new JavaClient();
		client.start_sync();
		client.start_async();
	}
}
