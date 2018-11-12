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

def find_build_root_dir():
	root_dir = get_pwd() 
	if root_dir.endswith('/'):
		root_dir = root_dir[:-1]
	while root_dir and root_dir != "/":
		if os.path.isfile(os.path.join(root_dir, "ROOT")):
			break
		root_dir = os.path.dirname(root_dir)
	if not root_dir or root_dir == "/":
		error_exit("Can't find BUILD_ROOT")
	return root_dir

root_dir = ""
build_option = ""
options = ["-c", "debug"]
c_flags = ""
if __name__ = "__main__":
	if len(sys.argv) > 1:
		if sys.argv[1] in options:
			build_option = sys.argv[1]
		else:
			error_exit("Invalid arguments")
	root_dir = find_build_root_dir()
	os.system("scons " + build_option)
    
