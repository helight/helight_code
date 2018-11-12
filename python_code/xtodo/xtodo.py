#!/usr/bin/env python
# -*- coding: utf-8 -*-
VERSION = "zqbtodo.py v11.07.2"
'''简明TODO 示例服务:
    - 110702 完成TODO条目的:增加/完成/重启/修订/删除/历史/整体变更历史 所有核心功能!
    - 110701 init.
依赖:Bottle 0.95 以上版本
    - 已经包含在工程目录中,可以不用安装
    - 一定要安装的话: pip install bottle
'''
import sys,pickle
import time
import base64,hashlib
import subprocess
import bsddb as bdb

from bottle import *

class Borg():
    '''base http://blog.youxu.info/2010/04/29/borg
        - 单例配置收集类
    '''
    __collective_mind = {}
    def __init__(self):
        self.__dict__ = self.__collective_mind
    #all kinds of configures::
    urlprefix   = ""
    dbtodo     = "mytodo.db"
    db = bdb.btopen(dbtodo, 'c')
    STATE = {"C>":"创建"
        ,"N>":"完成"
        ,"D>":"删除"
        ,"E>":"修订"
        ,"R>":"重启"
        }
    # 初始化TODO变更历史列表
    if db.has_key("BDBhistoric"):
        pass
    else:
        db["BDBhistoric"] = pickle.dumps([])
    # 初始化在待办列表
    if db.has_key("BDBdotolist"):
        pass
    else:
        db["BDBdotolist"] = pickle.dumps([])
    # 初始化已完成列表
    if db.has_key("BDBdonelist"):
        pass
    else:
        db["BDBdonelist"] = pickle.dumps([])
    # 初始化已删除列表
    if db.has_key("BDBdellist"):
        pass
    else:
        db["BDBdellist"] = pickle.dumps([])

# init all var
ini = Borg()

@route('%s/index.html'%ini.urlprefix)
@route('%s/'%ini.urlprefix)
def index():
    '''默认首页,根据两个序列值来展示所有条目
    '''
    return template('index.tpl') + template("dolist.tpl"
        ,todolist=__loadasdict(pickle.loads(ini.db["BDBdotolist"]))
        ,donelist=__loadasdict(pickle.loads(ini.db["BDBdonelist"]))
        )
def __loadhisdict(idlist):
    '''从id列表,逐一查询出对应的条目内容,返回标准字典数据对象:
        {'id':条目BDB键
        ,'state':条目变更操作
        ,'stamp':条目时间戮
        ,'txt':条目内容
        }
    '''
    datalist = []
    for i in idlist:
        item = {}
        item['id'] = i[2:]
        item['scode'] = i[:1]
        item['state'] = ini.STATE[i[:2]]
        item['stamp'] = time.strftime("%y%m%d %H:%M:%S",time.localtime(float(ini.db[i[2:]][:16])))
        item['txt'] = base64.urlsafe_b64decode(ini.db[i[2:]][17:])
        datalist.append(item)    
    return datalist[::-1]



def __loadasdict(idlist):
    '''从id列表,逐一查询出对应的条目内容,返回标准字典数据对象:
        {'id':条目BDB键
        ,'txt':条目内容
        ,'stamp':条目时间戮
        }
    '''
    todolist = []
    for i in idlist:
        item = {}
        item['id'] = i
        item['stamp'] = time.strftime("%y%m%d %H:%M:%S",time.localtime(float(ini.db[i][:16])))
        item['txt'] = base64.urlsafe_b64decode(ini.db[i][17:])
        todolist.append(item)
    return todolist




def __flushBDB(sequkey,dotype,val):
    '''根据键和新增值,对BDB 进行更新:
        - dotype ~ add|pop
    '''
    # 检查键值对是否存在,若没有,就创建
    if ini.db.has_key(sequkey):
        crtlist = pickle.loads(ini.db[sequkey])
    else:
        crtlist = []
        ini.db[sequkey] = pickle.dumps(crtlist)
    # 根据操作类型对指定键的值进行操作
    if 'pop' == dotype:
        crtlist.pop(crtlist.index(val))
    else:
        crtlist.append(val)
    ini.db[sequkey] = pickle.dumps(crtlist)




@route('%s/add'%ini.urlprefix, method='POST')
def add():
    '''条目增补:
        - 创建待办任务条目
        - 将条目ID 追加到 BDBdotolist 中
            - 创建条目编辑历史序列键('ver:原条目ID'),值条目ID
        - 同时归档到 BDBhistoric 中
    '''
    cnt = request.forms.get('txtadd')
    bdbkey = hashlib.sha224("%s+%s"%(int(time.time()),cnt)).hexdigest()
    verkey = "ver:%s"%bdbkey
    ini.db[bdbkey] = "%s@%s"%("%.5f"%time.time(),base64.urlsafe_b64encode(cnt))

    __flushBDB('BDBdotolist','add',bdbkey)
    __flushBDB(verkey,'add',bdbkey)
    __flushBDB('BDBhistoric','add','C>%s'%bdbkey)

    return template('index.tpl') + template("dolist.tpl"
        ,todolist=__loadasdict(pickle.loads(ini.db["BDBdotolist"]))
        ,donelist=__loadasdict(pickle.loads(ini.db["BDBdonelist"]))
        ,debug=bdbkey
        )
@route('%s/edit/:itemid'%ini.urlprefix)
def edit(itemid):
    '''条目修订:
        - 创建任务条目修订输入框,向fix 提交
        - 将条目ID 追加到 BDBdotolist 中
        - 同时归档到 BDBhistoric 中
    '''
    itemcnt = ini.db[itemid]
    txt = base64.urlsafe_b64decode(itemcnt[17:])
    #print ini.db[itemid][:16]
    stamp = time.strftime("%y%m%d %H:%M:%S",time.localtime(float(ini.db[itemid][:16])))
    return template('edit.tpl'
        ,itemtxt=txt
        ,crtid=itemid
        ,debug=stamp
        ) + template('dolist.tpl',
		todolist=__loadasdict(pickle.loads(ini.db["BDBdotolist"])),
		donelist=__loadasdict(pickle.loads(ini.db["BDBdonelist"]))
		)

@route('%s/fix'%ini.urlprefix, method='POST')
def fix():
    '''条目变更:
        - 先将原先内容复制成 备案条目,并将备案条目ID 归档到 Key='ver:原条目ID' 的版本记录中
        - 将原条目ID 内容更新
        - 同时归档到 BDBhistoric 中
    '''
    cnt = request.forms.get('txtadd')
    orgid = request.forms.get('crtid')
    # 生成版本条目,并复制入内容
    verkey = "ver:%s"%orgid
    bkupid = hashlib.sha224("%s+%s"%(int(time.time()),cnt)).hexdigest()
    ini.db[bkupid] = ini.db[orgid]
    __flushBDB(verkey,'add',bkupid)
    # 更新当前条目内容
    ini.db[orgid] = "%s@%s"%("%.5f"%time.time(),base64.urlsafe_b64encode(cnt))
    #__flushBDB('BDBdotolist','add',bdbkey)
    __flushBDB('BDBhistoric','add','E>%s'%orgid)

    return template('index.tpl') + template("dolist.tpl"
        ,todolist=__loadasdict(pickle.loads(ini.db["BDBdotolist"]))
        ,donelist=__loadasdict(pickle.loads(ini.db["BDBdonelist"]))
        ,debug=orgid
        )
@route('%s/done/:itemid'%ini.urlprefix)
def done(itemid):
    '''标记条目已经完成:
        - 将条目ID 从 BDBdotolist 中 pop()
        - 将条目ID 注入 BDBdonelist 中
        - 同时归档到 BDBhistoric 中
    '''

    __flushBDB('BDBdotolist','pop',itemid)
    __flushBDB('BDBdonelist','add',itemid)
    __flushBDB('BDBhistoric','add','N>%s'%itemid)

    return template('index.tpl') + template("dolist.tpl"
        ,todolist=__loadasdict(pickle.loads(ini.db["BDBdotolist"]))
        ,donelist=__loadasdict(pickle.loads(ini.db["BDBdonelist"]))
        ,debug=itemid
        )


@route('%s/redo/:itemid'%ini.urlprefix)
def redo(itemid):
    '''标记条目重新待办:
        - 将条目ID 从 BDBdonelist 中 pop()
        - 将条目ID 注入 BDBdotolist 中
        - 同时归档到 BDBhistoric 中
    '''

    __flushBDB('BDBdotolist','add',itemid)
    __flushBDB('BDBdonelist','pop',itemid)
    __flushBDB('BDBhistoric','add','R>%s'%itemid)

    return template('index.tpl') + template("dolist.tpl"
        ,todolist=__loadasdict(pickle.loads(ini.db["BDBdotolist"]))
        ,donelist=__loadasdict(pickle.loads(ini.db["BDBdonelist"]))
        ,debug=itemid
        )


@route('%s/dele/:itemid'%ini.urlprefix)
def dele(itemid):
    '''标记条目删除:
        - 将条目ID 从 BDBdonelist 中 pop()
        - 将条目ID 注入 BDBdellist 中
        - 同时归档到 BDBhistoric 中
    '''
    __flushBDB('BDBdotolist','pop',itemid)
    __flushBDB('BDBdellist','add',itemid)
    __flushBDB('BDBhistoric','add','D>%s'%itemid)

    return template('index.tpl') + template("dolist.tpl"
        ,todolist=__loadasdict(pickle.loads(ini.db["BDBdotolist"]))
        ,donelist=__loadasdict(pickle.loads(ini.db["BDBdonelist"]))
        ,debug=itemid
        )


@route('%s/hist/:itemid'%ini.urlprefix)
def hist(itemid):
    '''条目变更历史列表
    '''
    #print ini.db.keys()
    return template('hist.tpl'
        ,histlist=__loadasdict(pickle.loads(ini.db["ver:%s"%itemid]))[::-1]
        ,debug=itemid
        )
@route('%s/historic'%ini.urlprefix)
def historic():
    '''TODO 变更历史
    '''
    #print ini.db.keys()
    return template('historic.tpl'
        ,historiclist=__loadhisdict(pickle.loads(ini.db["BDBhistoric"]))
        ,debug="BDBhistoric"
        )
def __redirect(msgtype,msgcnt):
    redirect("%s/notify/%s/%s"%(ini.urlprefix
        ,msgtype
        ,base64.urlsafe_b64encode(msgcnt))
        )




@route('%s/notify/:msgtype/:msgcnt'%ini.urlprefix)
def notify(msgtype,msgcnt):
    exp = base64.urlsafe_b64decode(msgcnt)
    return template('notify',alert=msgtype,allmsg=exp,urlprefix=ini.urlprefix)


@error(404)
def error404(error):
    return '404 !-( Nothing here, sorry'

'''
@route('/favicon.ico')
@route('%s/favicon.ico'%ini.urlprefix)
def favicon():
    abort(204, "Nothing here.")
    #return 'Nothing here, sorry'
'''

debug(True)
#for uWSGI deploy server,close run self...
run(host='0.0.0.0', port=9126,reloader=True)

# Do NOT use bottle.run() with mod_wsgi
'''
import os
os.chdir(os.path.dirname(__file__))
import bottle
application = bottle.default_app()
'''


