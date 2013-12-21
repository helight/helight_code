#!Python
# -*- coding: utf-8 -*-
import sys
import json, sqlite3
#from bottle import route, run, debug, template, request, send_file, error
from bottle import * 

# only needed when you run Bottle on mod_wsgi
from bottle import default_app

class dbman:
	conn = 0
	cur = 0
	def __init__(self):
		self.conn = sqlite3.connect('todo.db')
		self.cur = self.conn.cursor()

	def execute(self, m_sql):
		self.cur.execute(m_sql)
		self.conn.commit()

	def fatchall(self, m_sql):
		self.cur.execute(m_sql)
		ret = self.cur.fetchall()
		return ret

	def __del__(self):
		self.conn.close()

dbop = dbman();
def to_index():
	todo = dbop.fatchall("select id, task, time from xtodo where status = 1")
	done = dbop.fatchall("select id, task, time from xtodo where status = 0")
	ret = template('index', todolist = todo, donelist = done)
	return ret

def get_all_task():
	ret = []
	todo = dbop.fatchall("select id, task, status, time from xtodo where status = 1")
	done = dbop.fatchall("select id, task, status, time from xtodo where status = 0")
	ret.append(todo)
	ret.append(done)
	return json.dumps(ret)

@route('/')
def index():
	return to_index()
@route('/get_all_task')
def all_task():
	return get_all_task()

@route('/add', method = 'POST')
def add_new():
	if request.forms.get('txtadd').strip():
		new = request.forms.get('txtadd').strip()
		dbop.execute("INSERT INTO xtodo (task,status) VALUES ('%s', %s)"% (new, 1))
		return get_all_task()

@route('/done/:itemid', method = 'GET')		
def done_this(itemid):
	if itemid.strip():
		dbop.execute("update xtodo set status = 0 where id = %s" % itemid)
	return get_all_task()

@route('/redo/:itemid', method = 'GET')
def redo_this(itemid):
	if itemid.strip():
		dbop.execute("update xtodo set status = 1 where id = %s" % itemid)
	return get_all_task()

@route('/del/:itemid', method = 'GET')
def redo_this(itemid):
	if itemid.strip():
		dbop.execute("delete from xtodo where id = %s" % itemid)
	return get_all_task()


@route('/static/<filename:path>')
def server_static(filename):
	print filename
	return static_file(filename, root='static/')

@route('/edit/:no', method='GET')
def edit_item(no):

    if request.GET.get('save','').strip():
        edit = request.GET.get('task','').strip()
        status = request.GET.get('status','').strip()

        if status == 'open':
            status = 1
        else:
            status = 0

        conn = sqlite3.connect('todo.db')
        c = conn.cursor()
        c.execute("UPDATE todo SET task = ?, status = ? WHERE id LIKE ?", (edit,status,no))
        conn.commit()
        conn.close()

        return '<p>The item number %s was successfully updated</p>' %no

    else:
        conn = sqlite3.connect('todo.db')
        c = conn.cursor()
        c.execute("SELECT task FROM todo WHERE id LIKE ?", no)
        cur_data = c.fetchone()
        conn.close()

        return template('edit_task', old = cur_data, no = no)

@route('/item:item#[1-9]+#')
def show_item(item):

        conn = sqlite3.connect('todo.db')
        c = conn.cursor()
        c.execute("SELECT task FROM todo WHERE id LIKE ?", item)
        result = c.fetchall()
        conn.close()

        if not result:
            return 'This item number does not exist!'
        else:
            return 'Task: %s' %result[0]

@route('/help')
def help():
	pass	
#send_file('help.html', root='.')

@route('/json:json#[1-9]+#')
def show_json(json):

    conn = sqlite3.connect('todo.db')
    c = conn.cursor()
    c.execute("SELECT task FROM todo WHERE id LIKE ?", json)
    result = c.fetchall()
    conn.close()

    if not result:
        return {'task':'This item number does not exist!'}
    else:
        return {'Task': result[0]}


@error(403)
def mistake403(code):
    return 'There is a mistake in your url!'

@error(404)
def mistake404(code):
    return 'Sorry, this page does not exist!'


debug(True)
run(reloader=True)
#remember to remove reloader=True and debug(True) when you move your application from development to a productive environment

