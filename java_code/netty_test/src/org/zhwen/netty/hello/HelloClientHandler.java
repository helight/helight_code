package org.zhwen.netty.hello;

import org.zhwen.netty.hello.Hello.Response;

import io.netty.channel.ChannelHandlerContext;
import io.netty.channel.SimpleChannelInboundHandler;

public class HelloClientHandler extends SimpleChannelInboundHandler<Response> {
	@Override
	protected void channelRead0(ChannelHandlerContext ctx, Response res) throws Exception {

		System.out.println("Server say : " + res.getResult());
		System.out.println("Server say : " + res.getSeqid());
	}

	@Override
	public void channelActive(ChannelHandlerContext ctx) throws Exception {
		System.out.println("Client active ");
		super.channelActive(ctx);
	}

	@Override
	public void channelInactive(ChannelHandlerContext ctx) throws Exception {
		System.out.println("Client close ");
		super.channelInactive(ctx);
	}
}
