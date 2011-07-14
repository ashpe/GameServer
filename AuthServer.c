/*
 ============================================================================
 Name        : GameServer.c
 Author      : Ashley Pope 
 Version     : -1.0
 Copyright   : ...nobody would copy this (i hope)
 Description : Basic game server
 ============================================================================
 */

#include "AuthConf.h"
#include "AuthServer.h"


int main(void) {
	int		   listenfd;
	pthread_t	   tid;
	sockaddr_in servaddr;
        addrinfo *result;

	listenfd = socket(AF_INET, SOCK_STREAM, 0);

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(SERV_PORT);

        result = getAddrInfo();

        if ( bind( listenfd, (sockaddr *) &servaddr, sizeof(servaddr) ) == -1) {
		puts("bind() error");
		exit(0);
	}

	printf("Listening for client connections @ %d..\n", SERV_PORT);
	
        listen(listenfd, LISTENQ);
        socklen_t addrlen = result->ai_addrlen;
        sockaddr_in *cliaddr = (sockaddr_in *) malloc( addrlen );
	for ( ; ; ) {
		socklen_t len=addrlen;
                ThreadParams *tp = new ThreadParams;
		tp->connfd = accept(listenfd, (sockaddr *) &cliaddr, &len);
		pthread_create(&tid, NULL, &init_thread, tp);
	}
}

static void * init_thread(void *arg) {

    ThreadParams *tp = new ThreadParams;
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
        
        for ( ; ; ) {
            if (recv(sockfd, buf, sizeof buf, n)) {
                puts(buf);
                memset(buf, 0, sizeof(buf));
                sleep(5);
                send(sockfd, "ping!?", 6, 0);
            }
        }

}

addrinfo* getAddrInfo() {
    
    int n;
    addrinfo hints, *result;
    
    bzero(&hints, sizeof(addrinfo));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    if ( (n = getaddrinfo("localhost", "5558", &hints, &result)) != 0) {
            perror("error getting address info");
    }

    return result;
}

