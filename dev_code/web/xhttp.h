/**
 * @author Helight.Xu@gmail.com
 * @date 2012.03.10 ~
 *
 * head file for xhttp.cc
 */

#ifndef  __XHTTP_HEAD__
#define __XHTTP_HEAD__
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <netdb.h>
#include <string>
#include <cerrno>
#include <iostream>

using namespace std;

class xhttp
{
public:
	xhttp(const char *hostname, const char *reqfile);
	~xhttp();
public:
	bool connet();
	string getdata();
	string postdata(string postdata);
	void close();

private:
	void mkreqdata(string &reqstr);
	int send_tcpdata(int sockfd, const char* sendbuf, size_t len);
	void mk_postdata(string &reqstr, int len);
	int recv_httpdata(int dwSockFd, char* pcRecvBuf, size_t tCount, size_t tMinCount);

private:
	int sockfd;

	string hostname;
	string hostip;

	string reqfile;
	string recvdate;

private:
	string get_head;
	string post_head1;
	string post_head2;
	string post_head3;
};
#endif
