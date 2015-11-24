package org.zhwen.netty.hello;

import java.net.InetAddress;


import org.zhwen.netty.hello.Hello.ReqestType;
import org.zhwen.netty.hello.Hello.Request;
import org.zhwen.netty.hello.Hello.Response;

import io.netty.channel.ChannelHandlerContext;
import io.netty.channel.SimpleChannelInboundHandler;

public class HelloServerHandler extends SimpleChannelInboundHandler<Request> {

	private int count = 0;
	@Override
	protected void channelRead0(ChannelHandlerContext ctx, Request req) throws Exception {
		Response.Builder response = Response.newBuilder();
		
		response.setSeqid(++count);
		response.setType(req.getType());
		response.setRet(0);
		if (ReqestType.LOGIN == req.getType()) {
			response.setResult(req.getLogin().getPswd());
			
			// 收到消息直接打印输出
			System.out.println(ctx.channel().remoteAddress() + " Say : " + req.getLogin().getUser()
					+ " name: " + req.getLogin().getPswd());
		}
		// 返回客户端消息 - 我已经接收到了你的消息
		// ctx.writeAndFlush("Received your message: " + msg + "\n");
		ctx.writeAndFlush(response.build());
	}

	/*
	 * 
	 * 覆盖 channelActive 方法 在channel被启用的时候触发 (在建立连接的时候)
	 * 
	 * channelActive 和 channelInActive 在后面的内容中讲述，这里先不做详细的描述
	 */
	@Override
	public void channelActive(ChannelHandlerContext ctx) throws Exception {

		System.out.println("RamoteAddress : " + ctx.channel().remoteAddress() + " active !");

		ctx.writeAndFlush("Welcome to " + InetAddress.getLocalHost().getHostName() + " service!\n");

		super.channelActive(ctx);
	}
	
	public void exceptionCaught(ChannelHandlerContext ctx,Throwable cause) { 
		// cause.printStackTrace();//捕捉异常信息
		System.out.println("RamoteAddress : " + ctx.channel().remoteAddress() + " closed !");
		ctx.close();//出现异常时关闭channel 
	} 
}