/*
 * Helight Simple Server
 *
 * Copyright (c) 2008-~  http://zhwen.org
 *
 * This source code is released for free distribution under the terms of the
 * GNU General Public License.
 *
 * file name: simple.c
 * modify date: 2008-11-10
 * Author: helight@zhwen.org
 *
 * Program definitions:
 * Simeple Server
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include <time.h>
#include <sys/epoll.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/mman.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <netdb.h>
#include <pthread.h>
#include <errno.h>
 
//thread num
#define THREAD_MAX 1
 
#define LISTEN_PORT 8000
 
//LISTEN_PORT-->LISTEN_PORT+LISTEN_MAX-1
#define LISTEN_MAX 1
 
#define SERVER_IP "127.0.0.1"

pthread_mutex_t mutex;//互斥对象
pthread_mutexattr_t mutexattr;//互斥对象属性

typedef struct
{
    char    ip4[128];
    int     port;
    int     fd;
} listen_info;

static listen_info s_listens;

static int init_listen_ip4(char *ip4, int port, int max_link);
static void server_proxy(char *ptr);
static void task_process(char *ptr);


int main(int argc, char *argv[])
{
	int fd, shm_id;
	char *addnum="xsharam";
	char *ptr;
	
	pthread_mutexattr_init(&mutexattr);//初始化互斥对象属性
	pthread_mutexattr_setpshared(&mutexattr,PTHREAD_PROCESS_SHARED);
	//设置互斥对象为PTHREAD_PROCESS_SHARED共享，即可以在多个进程的线程访问,
	//PTHREAD_PROCESS_PRIVATE为同一进程的线程共享
	pthread_mutex_init (&mutex, &mutexattr);
	
	fd = fork();
	switch(fd) {
	case -1:
		perror("main fork error:");
		exit(-1);
		break;
	case 0:
		shm_id = shm_open(addnum,O_RDWR,0);
		ftruncate(shm_id,sizeof(int));
		ptr = mmap(NULL,sizeof(int),PROT_READ|PROT_WRITE,MAP_SHARED,shm_id,0);/*连接共享内存区*/
		
		task_process(ptr);
		printf("this is child\n");
		break;
	default:
		shm_id = shm_open(addnum,O_RDWR|O_CREAT,0644);
		ftruncate(shm_id,sizeof(int));
		ptr = mmap(NULL,sizeof(int),PROT_READ|PROT_WRITE,MAP_SHARED,shm_id,0);/*连接共享内存区*/
		
		server_proxy(ptr);
		
		shm_unlink(addnum);//删除共享名称
		munmap(ptr,sizeof(int));//删除共享内存
	}
	return 0;
}

static int init_listen_ip4(char *ip, int port, int max_link)
{
	int            sock_listen;
	struct sockaddr_in    addr;
	unsigned int        optval;
	struct linger        optval1;

	bzero(&addr, sizeof(addr));
	inet_pton(AF_INET, ip, &(addr.sin_addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);

	sock_listen = socket(AF_INET, SOCK_STREAM, 0);
	if (0 > sock_listen) return(-1);

	//设置SO_REUSEADDR选项(服务器快速重起)
	optval = 0x1;
	setsockopt(sock_listen, SOL_SOCKET, SO_REUSEADDR, &optval, 4);

	//设置SO_LINGER选项(防范CLOSE_WAIT挂住所有套接字)
	optval1.l_onoff = 1;
	optval1.l_linger = 60;
	setsockopt(sock_listen, SOL_SOCKET, SO_LINGER, &optval1, sizeof(struct linger));

	if (0 > bind(sock_listen, (struct sockaddr *)&addr, sizeof(addr)))
	{
		close(sock_listen);
		return(-1);
	}

	if (0 > listen(sock_listen, max_link))
	{
		close(sock_listen);
		return(-1);
	}

	return(sock_listen);
}
/*
 * 服务接受进程
 * 接收参数为共享内存指针 ptr
 */
static void server_proxy(char *ptr)
{
	int	i, rc;		
	int	sock_listen; 
	int	sock_cli;   

	socklen_t	addrlen;
	struct sockaddr_in	addr4;
	
	struct sockaddr_un address;
    int sockfd;
    int len;
    int bytes;
    int result;
    char ch_recv, ch_send;

    address.sun_family = AF_UNIX;
    strcpy (address.sun_path, "xux");
    len = sizeof (address);
    
	sprintf(s_listens.ip4, "%s", SERVER_IP);
	s_listens.port = LISTEN_PORT;
	
	rc = init_listen_ip4(s_listens.ip4, s_listens.port, 64);
	if (0 > rc)	{
		fprintf(stderr, "无法创建服务器监听于%s:%d\n", s_listens.ip4, s_listens.port);
		perror(NULL);
		exit(-1);
	}
	s_listens.fd = rc;

	for ( ; ; )	{
		int tmp = 0;
		
		printf("begin to listen\n");
		sock_listen = s_listens.fd;
		addrlen = sizeof(struct sockaddr_in);
		bzero(&addr4, addrlen);
		sock_cli = accept(sock_listen, (struct sockaddr *)&addr4, &addrlen);
		if (0 > sock_cli){
			fprintf(stderr, "接收客户端连接失败\n");
			continue;
		}
		pthread_mutex_lock(&mutex);      
		*ptr = 1;
		tmp = *ptr;
		pthread_mutex_unlock(&mutex);
		printf("connected\n");
		  /*创建socket,AF_UNIX通信协议,SOCK_STREAM数据方式*/
		if ((sockfd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
				perror ("socket");
				goto sagain;
		}
		/*向服务器发送连接请求*/
		result = connect (sockfd, (struct sockaddr *)&address, len);
		if (result == -1) {
			printf ("ensure the server is up\n");
			perror ("connect");
			goto sagain;//exit (EXIT_FAILURE);
		}

		for (i = 0, ch_send = 'A'; i < 5; i++, ch_send++) {
			if ((bytes = write(sockfd, &ch_send, 1)) == -1) { /*发消息给服务器*/
				perror ("write");
				continue;
			}

			if ((bytes = read (sockfd, &ch_recv, 1)) == -1) { /*接收消息*/
				perror ("read");
				goto sagain;
			}

			printf ("receive from server data is %c\n", ch_recv);
		}		
sagain:	while(tmp != 0){
			pthread_mutex_lock(&mutex);      
			tmp = *ptr;
			pthread_mutex_unlock(&mutex);
		}
		close (sockfd);
		close(sock_cli);
	
	}//end of for(;;)

	exit(0);	
}

/*
 * 事物处理进程
 * 接收参数为共享内存指针 ptr
 */
static void task_process(char *ptr)
{
	int tmp = 0;
	int server_sockfd, client_sockfd;
	int server_len, client_len;
	struct sockaddr_un server_address;      /*声明一个UNIX域套接字结构*/
	struct sockaddr_un client_address;
	int i, bytes;
	char ch_send, ch_recv;

	unlink ("xux");       /*删除原有server_socket对象*/

	/*创建 socket, 通信协议为AF_UNIX, SCK_STREAM 数据方式*/
	server_sockfd = socket (AF_UNIX, SOCK_STREAM, 0);
	server_address.sun_family = AF_UNIX;/*配置服务器信息(通信协议)*/
	strcpy (server_address.sun_path, "xux");/*配置服务器信息(socket 对象)*/
	server_len = sizeof (server_address);/*配置服务器信息(服务器地址长度)*/
	/*绑定 socket 对象*/
	bind(server_sockfd, (struct sockaddr *)&server_address, server_len);
	listen(server_sockfd, 5);/*监听网络,队列数为5*/
	printf("Server is waiting for client connect...\n");

	client_len = sizeof (struct sockaddr_un);
	for ( ; ; )	{
		
		client_sockfd = accept (server_sockfd, (struct sockaddr *)&client_address, (socklen_t *)&client_len);
		if (client_sockfd == -1) {
			perror ("accept");
			goto tagain;
		}
		pthread_mutex_lock(&mutex);      
		tmp = *ptr;
		pthread_mutex_unlock(&mutex);
		
		while(tmp != 1){
			pthread_mutex_lock(&mutex);      
			tmp = *ptr;
			pthread_mutex_unlock(&mutex);
		}
		
		printf("I know, I will do it\n");
		for (i = 0, ch_send = '1'; i < 5; i++, ch_send++) {
			if ((bytes = read (client_sockfd, &ch_recv, 1)) == -1) {
					perror ("read");
					goto tagain;
			}
			printf ("The character receiver from client is %c\n", ch_recv);
			if ((bytes = write (client_sockfd, &ch_send, 1)) == -1) {
					perror ("read");
					goto tagain;
			}
		}
		
tagain:	pthread_mutex_lock(&mutex);      
		*ptr = 0;
		pthread_mutex_unlock(&mutex);	
		close (client_sockfd);
	}
	close(server_sockfd);
	unlink ("xux");
}
