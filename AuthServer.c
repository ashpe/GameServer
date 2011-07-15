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

using namespace std;

int main(void) {
	int		   listenfd;
	pthread_t	   tid;
	sockaddr_in servaddr;
        addrinfo *result;

	listenfd = socket(AF_INET, SOCK_STREAM, 0);

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(AUTH_PORT);

        result = getAddrInfo();

        if ( bind( listenfd, (sockaddr *) &servaddr, sizeof(servaddr) ) == -1) {
		puts("bind() error");
		exit(0);
	}

	printf("Listening for client connections @ %d..\n", AUTH_PORT);
	
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
    AuthDatabaseHandler dbh("auth_database");
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
        for ( ; ; ) {
            if (recv(sockfd, buf, sizeof buf, n)) {
                printf("DEBUG: %s", buf);
                if (strstr(buf,"LOGIN")) {
                    resp = check_login(buf, dbh);
                } else if (strcmp(buf,"PONG") == 0 || strcmp(buf,"L_OK") == 0) {
                    sleep(5);
                    resp = "PING";
                }

                bzero(buf, sizeof(buf));

                send(sockfd, resp.c_str(), resp.length(), 0);
            }
        }

}

string check_login(char *login_string, AuthDatabaseHandler dbh) {

    string extract_details = login_string;
    char *username = "root";
    char *password = "";
    
    cout << "Login details; " << extract_details << "\n";

    if (dbh.auth_user(username, password) == 1) {
        return "LOGIN:UNIQUE_KEY_GOES_HERE";
    } else {
        return "DIE";
    }
    
}

addrinfo* getAddrInfo() {
    
    int n;
    addrinfo hints, *result;
    
    bzero(&hints, sizeof(addrinfo));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    if ( (n = getaddrinfo("localhost", "5554", &hints, &result)) != 0) {
            perror("error getting address info");
    }

    return result;
}

