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
    tp->connfd = *((int *) arg);
    pthread_detach(pthread_self());
    read_data(tp->connfd);
    close(tp->connfd);
    delete tp;
    return (NULL); 

}

void read_data(int sockfd) {
        
	ssize_t n;
	char buf[MAXLINE];
        char* resp;
        for ( ; ; ) {
            if (recv(sockfd, buf, sizeof buf, n)) {
                
                if (strstr(buf, "LOGIN")) {
                    printf("Checking user exists: %s\n", buf);
                    resp = check_login(buf);
                } else if (strstr(buf, "PONG")) {
                    sleep(5);
                    resp = "PING";
                }

                memset(buf, 0, sizeof(buf));
                send(sockfd, resp, sizeof(resp), 0);
            }
        }

}
char* check_login(char* login_string) {
    sqlite3 *conn;
    int error_check = 0;

    char *split;
    char *username = "ashpez";
    char *password = "pass";

    error_check = sqlite3_open("auth_database", &conn);

    if (error_check) {
        puts("Cannot open database");
        exit(0);
    }

    sqlite3_stmt *res;
    const char *tail;
    const char *sql_prep = "select * from auth_login where username=? and password=?";
    
    error_check = sqlite3_prepare_v2(conn, sql_prep, strlen(sql_prep), &res, &tail);
    sqlite3_bind_text(res, 1, username, -1, SQLITE_STATIC);
    sqlite3_bind_text(res, 2, password, -1, SQLITE_STATIC);
    
    error_check = sqlite3_step(res);

    sqlite3_finalize(res);
    sqlite3_close(conn);
    
    if (error_check == 100) {
        puts("User found.\n\n");
        char *unique_key = "Unique_key_goes_here";
        return unique_key;
    } else {
        puts("No records.\n\n");
        char *not_found = "DIE";
        return not_found;
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

