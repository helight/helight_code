/**
 * @author Helight.Xu@gmail.com
 * @date 2012.03.10 ~
 */
#ifndef     _X_STRING_TOOL_H__
#define     _X_STRING_TOOL_H__

#include <openssl/md5.h>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <iostream>
#include <fstream>

using namespace std;

class str_tools
{
public:
	static string str_md5(string str);
	static string file_md5(string file_name);
};

#endif // _X_STRING_TOOL_H__
