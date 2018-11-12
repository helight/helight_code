package org.zhwen.test;

import org.apache.thrift.TException;

public class HelloWorldImpl implements HelloWorldService.Iface {
	 
	public HelloWorldImpl() {
	}
 
	public String sayHello(String username) throws TException {
		String ret = "Hi," + username + " welcome to my zhwen.org";
		System.out.println(ret);
		return ret;
	}
 
}