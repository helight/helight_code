package org.zhwen.netty.hello;

import io.netty.channel.ChannelInitializer;
import io.netty.channel.ChannelPipeline;
import io.netty.channel.socket.SocketChannel;
import io.netty.handler.codec.protobuf.ProtobufDecoder;
import io.netty.handler.codec.protobuf.ProtobufEncoder;
import io.netty.handler.codec.protobuf.ProtobufVarint32FrameDecoder;
import io.netty.handler.codec.protobuf.ProtobufVarint32LengthFieldPrepender;


public class HelloServerInitializer extends ChannelInitializer<SocketChannel> {

	@Override
	protected void initChannel(SocketChannel ch) throws Exception {
		ChannelPipeline pipeline = ch.pipeline();

		// 以("\n")为结尾分割的 解码器
		// pipeline.addLast("framer", new DelimiterBasedFrameDecoder(8192, Delimiters.lineDelimiter()));

		// 字符串解码 和 编码
		// pipeline.addLast("decoder", new StringDecoder());
		// pipeline.addLast("encoder", new StringEncoder());
		
		//解码  
		pipeline.addLast("frameDecoder", new ProtobufVarint32FrameDecoder());  
		//构造函数传递要解码成的类型  
		pipeline.addLast("protobufDecoder", new ProtobufDecoder(Hello.Request.getDefaultInstance()));  
		//编码  
		pipeline.addLast("frameEncoder", new ProtobufVarint32LengthFieldPrepender());  
		pipeline.addLast("protobufEncoder", new ProtobufEncoder());  
		
		//业务逻辑处理  Handler
		pipeline.addLast("handler", new HelloServerHandler());
	}
}