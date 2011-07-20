/*
============================================================================
Name        : GameServer.c
Author      : Ashley Pope
Version     : -1.0
Copyright   : ...nobody would copy this (i hope)
Description : Basic game server
============================================================================
*/

#include "GameConf.h"
#include "GameServer.h"
#include "PacketHandler.h"

using namespace std;

int main(void) {
    int	  	   listenfd;
    pthread_t	   tid;
    sockaddr_in      servaddr;
    addrinfo         *result;

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
    for (;;) {
        socklen_t len=addrlen;
        ThreadParams *tp = new ThreadParams;
        tp->connfd = accept(listenfd, (sockaddr *) &cliaddr, &len);
        pthread_create(&tid, NULL, &init_thread, tp);
    }
}

static void * init_thread(void *arg) {

    ThreadParams *tp = new ThreadParams;
    AuthDatabaseHandler dbh((char*)AUTH_DATABASE);
    tp->connfd = *((int *) arg);
    pthread_detach(pthread_self());
    read_data(tp->connfd, dbh);
    close(tp->connfd);
    delete tp;
    return (NULL);

}

void read_data(int sockfd, AuthDatabaseHandler dbh) {

    ssize_t n;
    char buf[MAXLINE];
    string resp;
    PacketHandler pck;

    for (;;) {
        if (recv(sockfd, buf, sizeof buf, n)) {
            
            Packet::PacketHeader header;
            header.ParseFromString(buf);

            if (header.has_auth_code()) {
                puts("Checking auth code in database..");
                string auth_code = header.auth_code();
                if (dbh.auth_code((char *)auth_code.c_str()) == 1) {
                    resp = pck.connected_packet(true);
                    puts("Auth code validated, successfully connected to game server.");
                } else {
                    resp = pck.connected_packet(false);
                    puts("Error, auth code sucks. Exiting..");
                }
            }

            bzero(buf, sizeof(buf));

            if (resp.length() > 0) {
                send(sockfd, resp.c_str(), resp.length(), 0);
            }
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

