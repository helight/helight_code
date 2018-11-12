package org.zhwen.akka_test;

import akka.actor.ActorRef;
import akka.actor.ActorSystem;
import akka.actor.Props;

public class MainTest 
{
	
	public static void main(String[] args) 
	{
		System.out.println( "Hello World!" );
		
		ActorSystem system = ActorSystem.create("demo1");
		ActorRef actor1 = system.actorOf(Props.create(Actor01.class));
		ActorRef actor2 = system.actorOf(Props.create(Actor02.class));
		
		actor1.tell("hello akka Actor01!!", actor2);
		
		// actor1.tell("hello akka Actor01 001!!", actor1);
		actor2.tell("hello akka Actor02 002!!", actor1);
		
		actor2.tell("hello akka Actor02 002!!",  ActorRef.noSender());
		
		// actor2.ask(actor1, "request", 1000);
		system.shutdown();//
		
		System.out.println( "Bey World!" );
	}
	
}