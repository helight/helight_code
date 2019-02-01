# coding=utf8

import itchat
import requests

def get_response2(msg):    
    apiUrl = 'http://www.tuling123.com/openapi/api'
    data = {
            'key': 'd0860e4c209a4751815c867d5d37b5fd', # Tuling Key，API的值
            'info': msg, # 发出去的消息
            'userid': 'helight', # 用户名
            } 
    r = requests.post(apiUrl, data=data).json() # post请求
    return r.get('text')

def get_response(msg):
    # apiUrl = 'http://openapi.tuling123.com/openapi/api/v2' 
    data = {
        "reqType":0,
        "perception": {
            "inputText": {
                "text": "附近的酒店"
            }
        },
        "userInfo": {
            "apiKey": "d0860e4c209a4751815c867d5d37b5fd",
            "userId": "helight"
        }
    }
    r = requests.post(apiUrl, data=data).json() # post请求
    return r.get('text')

@itchat.msg_register(itchat.content.TEXT) # 用于接收来自朋友间的对话消息
def print_content(msg):
    return get_response(msg['Text'])
    
@itchat.msg_register([itchat.content.TEXT], isGroupChat=True) # 用于接收群里面的对话消息
def print_content(msg):
    return get_response(msg['Text'])

itchat.auto_login(True) # 通过微信扫描二维码登录
itchat.run()