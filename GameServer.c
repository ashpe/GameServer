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
#include <netinet/in.h>

#define SERV_PORT 5558
#define LISTENQ 32
#define MAXLINE 4096

void onSockRead(int sockfd);

int main(void) {

	int		listenfd, connfd;
	pid_t	childpid;
	socklen_t clilen, iplen;
	struct sockaddr_in cliaddr,servaddr, get_ip;

	listenfd = socket(AF_INET, SOCK_STREAM, 0);

	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(SERV_PORT);


	if ( bind(listenfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) == -1) {
		puts("bind() error");
		exit(0);
	}

	printf("Listening for client connections @ %d..\n", SERV_PORT);

	listen(listenfd, LISTENQ);

	for ( ; ; ) {
		clilen = sizeof(cliaddr);
		connfd = accept(listenfd, (struct sockaddr *) &cliaddr, &clilen);
		if ( (childpid = fork() ) == 0) {
			close(listenfd);
            send(connfd, "Hello, world!", 13, 0);
			onSockRead(connfd);
			exit(0);
		}
		close(connfd);
	}
}

void onSockRead(int sockfd) {

	ssize_t n;
	char buf[MAXLINE];

	recv(sockfd, buf, sizeof buf, n);

	puts(buf);

}

