#!/usr/bin/env python

import sys
import commands

def get_cpu_load (processor_number=0):
	"""Calculates the system load."""
	try:
		f = open("/proc/stat", "r")
		tmp = f.readlines(2000)
		f.close()
	except:
		print _("Failed to open /proc/stat")
		return None
	if processor_number == 0 : sufix = ''
	else: sufix = str(processor_number -1)
	line = tmp[processor_number]

	if line.startswith("cpu%s"% (sufix)):
		cuse = float( line.split()[1] )
		cn = float( line.split()[2] )
		csys = float( line.split()[3])
		if sufix == '':
			load = cuse + cn
		else:
			load = cuse + csys + cn
		#load = int(load / .update_interval)
		return load
	return None

def cpu_get_cpu_name():
	"""Returns Cpu Name"""
	try:
		f = open("/proc/cpuinfo", "r")
		tmp = f.readlines(500)
		f.close()
	except:
		print _("Failed to open /proc/cpuinfo")
		return None
		list = []
	for line in tmp:
		if line.startswith("model name"):
			return line.split(':')[1].strip()
	return ''

def net_get_updown():
	"""Returns upload and download"""
	try:
		f = open("/proc/net/dev", "r")
		data = f.readlines(2000)
		f.close()
		newNetUp = 0
		newNetDown = 0
		for i in data:
			if i.find(':') != -1 and i.strip().startswith('lo:') == False:
				v = i.split(':')[1].split()
				newNetUp = float( v[8] )+newNetUp
				newNetDown = float( v[0] )+newNetDown

	
		return (newNetUp/1024), (newNetDown/1024)
	except:
		print(_("Can't open /proc/net/dev"))
		return 0,0

def net_get_connections ():
	"""This will return the number of connections."""
	data = commands.getoutput("netstat -n | grep -c tcp")
	
	return data

def main():
	load = get_cpu_load()
	name = cpu_get_cpu_name()
	print str(name) + "  " + str(load)
	print net_get_updown()
	print net_get_connections()

if __name__ == "__main__":
	sys.exit(main())
