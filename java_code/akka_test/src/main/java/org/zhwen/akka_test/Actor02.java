package org.zhwen.akka_test;

import akka.actor.UntypedActor;

public class Actor02 extends UntypedActor {

	@Override
	public void onReceive(Object arg0) throws Exception {
		if(arg0 instanceof String)
			System.out.println("Actor02 -- onReceive -->" + arg0);		
	}

}