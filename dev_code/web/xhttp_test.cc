/**
 * @author Helight.Xu@gmail.com
 * @date 2012.03.10 ~
 */
#include <stdio.h>
//#include "thirdparty/gtest/gtest.h"
//#include "thirdparty/jsoncpp/json.h"
#include <zlib.h>
#include "xhttp.h"


#define BUFSIZE 102400
/*
 * test for 
 * xhttp   
 */
int main(int argc, char* argv[])
{
	char *requrl="/search_product.htm?user_action=initiative&at_topsearch=1&sort=st&type=p&userBucket=8&userBucketCell=28&q=iphone5";

	// xhttp qqget("list.tmall.com", requrl);
	xhttp qqget("www.laidingba.com", "/key/261661.html");
	qqget.connet();
	string retget = qqget.getdata();
	//inflateInit2(&strm, 47);
	//cout << retget << endl;
	std::cout << retget;

	qqget.close();
/*
	xhttp qqget2("zhwen.org", "/apps/tt.php");
	qqget2.connet();
	string retstr = qqget2.postdata("submit_command=save&require_auth=1\r\n\r\n");
	cout << retstr << endl;
	qqget2.close();
*/
	return 0;
}

