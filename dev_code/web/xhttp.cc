/**
 * @author Helight.Xu@gmail.com
 * @date 2012.03.10 ~
 */
#include "xhttp.h"
#include <stdlib.h>

/*
 * xhttp
 * @hname host name
 * @rfile reqest file or resource
 */
xhttp::xhttp(const char *hname, const char *rfile)
{
	struct hostent *site;

	site = gethostbyname (hname);

	hostname = hname;// = hname;
	hostip = inet_ntoa(*((struct in_addr *)site->h_addr));// = hip;
	reqfile = rfile;// = hreq;
	//printf("hostname: %s, IP: %s  reqfile: %s", hostname.c_str(), hostip.c_str(), reqfile.c_str());
	sockfd = -1;
	get_head = "GET /%s HTTP/1.1\r\nHost: %s\r\n";
	get_head.append("User-Agent: Mozilla/5.0 (Windows NT 6.1; rv:10.0.1) Gecko/20100101 Firefox/10.0.1\r\n");
	get_head.append("Accept: text/html,application/json,application/xml;q=0.9,*/*;q=0.8\r\n");
	get_head.append("Accept-Language: zh-cn,zh;q=0.5\r\n");
	get_head.append("Accept-Encoding: gzip, deflate\r\n");
	get_head.append("Connection: keep-alive\r\n\r\n");

	post_head1 = "POST /%s HTTP/1.1\r\nHost: %s\r\n";
	post_head2 = "User-Agent: Mozilla/5.0 (Windows NT 6.1; rv:10.0.1) Gecko/20100101 Firefox/10.0.1\r\n";
	post_head2.append("Accept: text/html,application/json,application/xml;q=0.9,*/*;q=0.8\r\n");
	post_head2.append("Content-Type: application/x-www-form-urlencoded; charset=UTF-8\r\n");
	post_head3 = "Content-Length: %d\r\nConnection: keep-alive\r\nPragma: no-cache\r\n\r\n";
/*
	get_head = "GET /search_product.htm?user_action=initiative&at_topsearch=1&sort=st&type=p&userBucket=8&userBucketCell=28&q=iphone5 HTTP/1.1\r\n";
	get_head.append("Host: list.tmall.com\r\n");
	get_head.append("User-Agent: Mozilla/5.0 (X11; Linux i686; rv:10.0.12) Gecko/20100101 Firefox/10.0.12 Iceweasel/10.0.12\r\n");
	get_head.append("Accept: text/html,application/xhtml+xml,application/xml;q=0.9,;q=0.8\r\n");
	get_head.append("Accept-Language: en-us,en;q=0.5\r\n");
	get_head.append("Accept-Encoding: gzip, deflate\r\n");
	get_head.append("Connection: keep-alive\r\n");
	get_head.append("Cookie: mpp=t%3D0%26m%3D%26h%3D0%26l%3D0; _tb_token_=8o6t3PfsBkwn; cookie2=821b9ecd976a4ea29907de06942f5142; t=0c6c340550934f17d03b27fe7b5555e0; res=scroll%3A1417*3891-client%3A1417*396-offset%3A1417*3892-screen%3A1440*900; cq=ccp%3D1; cna=89kPCuWSqk8CAXkOYi7YfFYy\r\n\r\n");
*/
}

/*POST /fcgi-bin/XRms.fcgi HTTP/1.1
Host: rms.isoso.com
User-Agent: Mozilla/5.0 (Windows NT 6.1; rv:14.0) Gecko/20100101 Firefox/14.0.1
*///Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8
/*Accept-Language: zh-cn,zh;q=0.8,en-us;q=0.5,en;q=0.3
Accept-Encoding: gzip, deflate
Connection: keep-alive
Referer: http://rms.isoso.com/show_web_project.html?rms_ver=4.1
Content-Length: 128
Content-Type: text/plain; charset=UTF-8
Cookie: ITIL_OA_NICK_NAME=helightxu; ITIL_OA_ENCRYPT="nSavgiaQn2Ndus8a4i3U9PK/0ManQWn6nRWo6v7CQi1UyqzIXSPaTgtKe1I1GAe6A87mNn3d7oqOSIApY+tLXQ=="; OM_DEPID=3716; OM_HASH=2626426852636859587; OM_TIME=1348471047; OM_USERNAME=helightxu; STROAGE_PRI=admin; RELEASE_PRI=admin; MONITOR_PRI=admin; OM_TOP_CLASS=0; OM_SUB_CLASS=0; OM_IS_WEB=1; OM_ALL_PROJECT_PAGE=0
Pragma: no-cache
Cache-Control: no-cache
*/

xhttp::~xhttp()
{
	if(sockfd){
		close();
	}
}

bool xhttp::connet()
{
	int len, result;
	struct sockaddr_in address;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr(hostip.c_str());
    address.sin_port = htons(80);
    len = sizeof(address);
    result = connect(sockfd, (struct sockaddr *)&address, len);
    if(result == -1){
		::close(sockfd);
        perror("oops: client1");
        return false;
    }
    // cout << "connet ok" << endl;
    return true;
}

void xhttp::mkreqdata(string &reqstr)
{
	char buff[1024*4];

	snprintf(buff, sizeof(buff), get_head.c_str(), reqfile.c_str(), hostname.c_str());
	//printf("\nsend data: %s\n", buff);
	reqstr = buff;
}

string xhttp::getdata()
{
	char buff[1024*2];
	char *ch = NULL;
	string reqstr;
	if(sockfd < 0)
		return NULL;
	mkreqdata(reqstr);
	//cout << reqstr << endl;
	send_tcpdata(sockfd, reqstr.c_str(), reqstr.length());

	recv_httpdata(sockfd, buff, sizeof(buff), 0);
	//printf("buff:%s\n", buff);
	ch = strstr(buff, "\r\n\r\n");
	if(ch) {
		recvdate = ch;
	}else {
		return NULL;
	}
	//printf("receive: %s\n", recvdate.c_str());

	return recvdate;
}

void xhttp::mk_postdata(string &reqstr, int len)
{
	char buff[1024*4];

	snprintf(buff, sizeof(buff), post_head1.c_str(), reqfile.c_str(), hostname.c_str());
	//printf("\nsend data: %s\n", buff);

	reqstr.append(buff);
	reqstr.append(post_head2);
	snprintf(buff, sizeof(buff), post_head3.c_str(), len);
	reqstr.append(buff);
}

string xhttp::postdata(string postdata)
{
	char buff[1024*2];
	char *ch = NULL;
	string reqstr;
	if(sockfd < 0)
		return NULL;
	mk_postdata(reqstr, postdata.length());

	reqstr.append(postdata);
	//cout << reqstr << endl;
	send_tcpdata(sockfd, reqstr.c_str(), reqstr.length());

	recv_httpdata(sockfd, buff, sizeof(buff), 0);
	//printf("buff:%s\n", buff);
	ch = strstr(buff, "\r\n\r\n");
	if(ch) {
		recvdate = ch;
	}else {
		return NULL;
	}
	//printf("receive: %s\n", recvdate.c_str());

	return recvdate;
}

//发送tcp数据
int xhttp::send_tcpdata(int sockfd, const char* sendbuf, size_t len)
{
	int thisend;
	unsigned int sended = 0;

	if(len == 0)
		return 0;

	while(sended < len)	{
		do {
			thisend = (int)::send(sockfd, sendbuf, len-sended, 0);
		} while((thisend < 0) && (errno == EINTR));

		if(thisend < 0)	{
			string str = strerror(errno);
			return sended;
		}
		sended += thisend;
		sendbuf += thisend;
	}

	return (int)len;
}

void xhttp::close()
{
	::close(sockfd);
	sockfd = -1;
}

/** 接收tcp数据 */
int xhttp::recv_httpdata(int dwSockFd, char* pcRecvBuf, size_t tCount, size_t tMinCount)
{
	char*  szbuff = pcRecvBuf;
	int dwBytesRead = 0;
	char* p = szbuff;
	int thiread;

	//while (dwBytesRead < tCount) {
		do {
			thiread = (int)::recv(sockfd, szbuff, tCount-dwBytesRead, 0);
			szbuff += thiread;
			dwBytesRead += thiread;
		} while((thiread < 0) && (errno == EINTR));
		//printf("thiread : %d, errno %d \n", thiread, errno);

		// cout << p << endl;
		char* q = strstr(p,"\r\n\r\n");
		if (q) {
			char* pLen = strstr(p,"Content-Length:");
			if (pLen && pLen < q) {
				pLen += 15;
				while(*pLen && (*pLen >'9'|| *pLen <'0'))pLen++;
				int dwContentLength = atoi(pLen);
				if (dwContentLength > 0 && dwContentLength != szbuff-q-4) {
					do {
						thiread = (int)::recv(sockfd, szbuff, tCount-dwBytesRead, 0);
						szbuff += thiread;
						dwBytesRead += thiread;
					} while((thiread < 0) && (errno == EINTR));
				}
			}
		}
		*szbuff = '\0';
		return dwBytesRead;
	//}
}
