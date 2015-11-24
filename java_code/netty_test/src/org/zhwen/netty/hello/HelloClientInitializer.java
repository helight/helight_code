package org.zhwen.netty.hello;

import io.netty.channel.ChannelInitializer;
import io.netty.channel.ChannelPipeline;
import io.netty.channel.socket.SocketChannel;
import io.netty.handler.codec.protobuf.ProtobufDecoder;
import io.netty.handler.codec.protobuf.ProtobufEncoder;
import io.netty.handler.codec.protobuf.ProtobufVarint32FrameDecoder;
import io.netty.handler.codec.protobuf.ProtobufVarint32LengthFieldPrepender;

public class HelloClientInitializer extends ChannelInitializer<SocketChannel> {
	@Override
	protected void initChannel(SocketChannel ch) throws Exception {
		ChannelPipeline pipeline = ch.pipeline();

		/*
		 * 这个地方的 必须和服务端对应上。否则无法正常解码和编码
		 * 
		 * 解码和编码 我将会在下一张为大家详细的讲解。再次暂时不做详细的描述
		 */
		// pipeline.addLast("framer", new DelimiterBasedFrameDecoder(8192, Delimiters.lineDelimiter()));
		// pipeline.addLast("decoder", new StringDecoder());
		// pipeline.addLast("encoder", new StringEncoder());
		//解码用  
		pipeline.addLast("frameDecoder", new ProtobufVarint32FrameDecoder());  
        //构造函数传递要解码成的类型  
		pipeline.addLast("protobufDecoder", new ProtobufDecoder(Hello.Response.getDefaultInstance()));  
        //编码用  
		pipeline.addLast("frameEncoder", new ProtobufVarint32LengthFieldPrepender());  
		pipeline.addLast("protobufEncoder", new ProtobufEncoder());  
        //业务逻辑用  
		pipeline.addLast("handler", new HelloClientHandler());
	}
}
