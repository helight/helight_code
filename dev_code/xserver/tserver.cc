#include <signal.h>
#include <time.h>
#include <stdarg.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/param.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <stddef.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <err.h>
#include <pthread.h>

#define MAXLINE 1024
#define SERV_PORT 8000
#define LISTENQ 1024
static int sockfd;
static FILE *fp;
int tcp_listen(const char* address, uint16_t port);

void str_echo (int sockfd )
{
    ssize_t n;
    char line[MAXLINE];
    for (; ; )
    {
        if ((n=read(sockfd, line, MAXLINE) )==0)
            return;
        write(sockfd, line, n);
    }
}

static void *doit ( void *arg)
{
    pthread_detach(pthread_self ( ) );
    str_echo ((int)arg);
    close ((int)arg);
    return NULL;
}

int setnonblock(int fd)
{
    int flags;

    flags = fcntl(fd, F_GETFL);
    if (flags < 0)
        return flags;
    flags |= O_NONBLOCK;
    if (fcntl(fd, F_SETFL, flags) < 0)
        return -1;

    return 0;
}

int main(int argc, char** argv)
{
    int listenfd, connfd;
    socklen_t addrlen,len;
    struct sockaddr_in cliaddr, servaddr;
    pthread_t tid;

    /*
       listenfd = socket(AF_INET, SOCK_STREAM, 0);
    //bzero(servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = SERV_PORT;
    bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr));
    listen(listenfd, LISTENQ);
    addrlen = sizeof(cliaddr);
    */
    listenfd = tcp_listen("127.0.0.1", 8088);
    printf("tsets\n");
    for( ; ; ){
        printf("xxxxxxx\n");
        //sleep(1);
        len = addrlen;
        connfd = accept(listenfd, (struct sockaddr*)&cliaddr, &len );
        pthread_create(&tid, NULL, &doit, ( void*)connfd);
    }
}
int tcp_listen(const char* address, uint16_t port)
{
    int sock;
    struct sockaddr_in listen_addr;
    int reuseaddr_on = 1;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
        err(1, "listen failed");
    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &reuseaddr_on,
                   sizeof(reuseaddr_on)) == -1)
        err(1, "setsockopt failed");
    memset(&listen_addr, 0, sizeof(listen_addr));
    listen_addr.sin_family = AF_INET;
    listen_addr.sin_addr.s_addr = inet_addr(address);
    listen_addr.sin_port = htons(port);

    if (bind(sock, (struct sockaddr *)&listen_addr,
             sizeof(listen_addr)) < 0)
        err(1, "bind failed");

    if (sock < -1) {
        return false;
    }

    if (listen(sock, 5) < -1) {
        return false;
    }
    //if (setnonblock(sock) < 0)
    //		err(1, "failed to set server socket to non-blocking init");
    return sock;
}

/*
   void * thread_main( void * arg )
   {
   for( ; ; ) {
   while( iget == iput ) sleep(1);//pthread_cond_wait( ...... );
   connfd = clifd[iget];     // 从队列中获得连接句柄[/b]
   if( ++iget == MAXNCLI ) iget = 0;
   web_child( connfd );
   close( connfd );
   }
   }

*/
