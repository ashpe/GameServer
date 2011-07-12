/*
 ============================================================================
 Name        : GameClient.c
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
int connect_to(const char* address, int port);

int main(void) {

	int sock = connect_to("localhost", SERV_PORT);
	puts("Connected successfully");

	send(sock, "Hello, world!", 13, 0);
	onSockRead(sock);
	exit(0);

}

int connect_to(const char* address, int port) {

	int		   sockfd;
	struct sockaddr_in servaddr;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	memset(&servaddr, 0, sizeof(servaddr));

	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(port);
	inet_pton(AF_INET, address, &servaddr.sin_addr);

	if (connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) == -1) {
		perror("Error connecting to host");
	} else {
		return sockfd;
	}

}

void onSockRead(int sockfd) {

	ssize_t n;
	char buf[MAXLINE];

	recv(sockfd, buf, sizeof buf, n);

	puts(buf);

}
