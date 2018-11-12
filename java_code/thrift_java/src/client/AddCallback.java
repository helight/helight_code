package client;

import org.apache.thrift.TException;
import org.apache.thrift.async.AsyncMethodCallback;

import rpc.tutorial.Calculator.AsyncClient.add_call;


public class AddCallback implements AsyncMethodCallback<add_call> {  
	  
    // 返回结果  
    @Override  
    public void onComplete(add_call response) {  
        System.out.println("onComplete");  
        try {  
            System.out.println(response.getResult());  
        } catch (TException e) {  
            e.printStackTrace();  
        }  
    }  
  
    // 返回异常  
    @Override  
    public void onError(Exception exception) {  
        System.out.println("onError");  
    }  
  
}
