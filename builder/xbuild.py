#!/usr/bin/env python
# Authors: HelightXu <helight@zhwen.org>

import os, sys, time, subprocess

def error_exit(msg, code = 1):
	msg = "build error: " + msg
	print >> sys.stderr, msg
	sys.exit(code)

def get_pwd():
	return subprocess.Popen(['pwd'],
			stdout = subprocess.PIPE,
			shell = True).communicate()[0].strip()

def find_build_root_dir(pwdir):
	root_dir = pwdir
	if root_dir.endswith('/'):
		root_dir = root_dir[:-1]
	while root_dir and root_dir != "/":
		if os.path.isfile(os.path.join(root_dir, "BUILD_ROOT")):
			break
		root_dir = os.path.dirname(root_dir)
	if not root_dir or root_dir == "/":
		error_exit("Can't find BUILD_ROOT")
	#print root_dir
	return root_dir

def create_scons_conf(root_dir, build_dir, subdir, flags):
	fd = open(root_dir + "/SConstruct", "w+")
	fd.write("env = Environment(CCFLAGS = '" + flags + "')\r\n")
	fd.write("Export(\"env\")\r\n")
	fd.write("SConscript('"+ subdir + "/SConscript', variant_dir='" + build_dir + subdir +"', duplicate=0)\r\n")
	fd.close()

root_dir = ""
build_option = ""
options = ["-c", "debug"]
c_flags = ""

if __name__ == "__main__":
	if len(sys.argv) > 1:
		if sys.argv[1] in options:
			build_option = sys.argv[1]
		else:
			error_exit("Invalid arguments")
	pwdir = get_pwd() 
	#print pwdir
	root_dir = find_build_root_dir(pwdir)
	if build_option == "debug":
		build_option = ""
		build_dir = root_dir + "/build-debug/"
		c_flags = "-g"
	else:
		c_flags = "-O2"
		build_dir = root_dir + "/build/"
	subdir = pwdir[len(root_dir) + 1:]
	#print subdir
	create_scons_conf(root_dir, build_dir, subdir, c_flags)
	os.chdir(root_dir)
	os.system("scons " + build_option)
