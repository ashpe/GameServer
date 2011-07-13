/*
 ============================================================================
 Name        : GameServer.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <pthread.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>

#define SERV_PORT 5558
#define LISTENQ 32
#define MAXLINE 4096

void onSockRead(int sockfd);
static void * init_thread(void *arg);
void getAddrInfo(struct addrinfo* results);
struct ThreadParams {
      int connfd;
};

void getAddrInfo(struct addrinfo* results) {
    
    int n;
    struct addrinfo hints, *result;
    
    bzero(&hints, sizeof(struct addrinfo));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    if ( (n = getaddrinfo("localhost", "5558", &hints, &result)) != 0) {
            perror("error getting address info");
    }

    *results=*result;

}

int main(void) {

	int		   listenfd;
	pthread_t	   tid;
	struct sockaddr_in servaddr;
        struct addrinfo *result;

	listenfd = socket(AF_INET, SOCK_STREAM, 0);

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(SERV_PORT);

        //getAddrInfo(result);

	if ( bind( listenfd, (struct sockaddr *) &servaddr, sizeof(servaddr) ) == -1) {
		puts("bind() error");
		exit(0);
	}

	printf("Listening for client connections @ %d..\n", SERV_PORT);
	
        listen(listenfd, LISTENQ);
        socklen_t addrlen = result->ai_addrlen;
        //struct sockaddr_in *cliaddr = (sockaddr_in *) malloc( addrlen );
	for ( ; ; ) {
		socklen_t len=addrlen;
                //struct ThreadParams *tp = new ThreadParams;
		//tp->connfd = accept(listenfd, (struct sockaddr *) &cliaddr, &len);
		//pthread_create(&tid, NULL, &init_thread, tp);
	}
}

static void * init_thread(void *arg) {

    struct ThreadParams *tp = new ThreadParams;
    tp->connfd = *((int *) arg);
    pthread_detach(pthread_self());
    onSockRead(tp->connfd);
    close(tp->connfd);
    delete tp;
    return (NULL); 

}

void onSockRead(int sockfd) {

	ssize_t n;
	char buf[MAXLINE];

	recv(sockfd, buf, sizeof buf, n);

	puts(buf);

}

