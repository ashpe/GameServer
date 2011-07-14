/*
 ============================================================================
 Name        : GameClient.c
 Author      : Ashley Pope
 Version     : -1.0
 Copyright   : as if you would copy this!
 Description : Game-client-thingy.
 ============================================================================
 */

#include "AuthConf.h"
#include "GameConf.h"
#include "GameClient.h"

int main(void) {
        
        char username[USERNAME_LEN];
        char password[PASSWORD_LEN];

        puts("**Login**");
        printf("Enter username: ");
        fgets(username, USERNAME_LEN, stdin);
        printf("Enter password: ");
        fgets(password, PASSWORD_LEN, stdin);
        
	int sock = connect_to("localhost", AUTH_PORT);
	puts("Connected successfully");
        send(sock, "S_OK", 4, 0);
        for( ; ; ) {

            read_data(sock);

        }
}

int connect_to(const char* address, int port) {

	int		   sockfd;
        sockaddr_in        servaddr;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	memset(&servaddr, 0, sizeof(servaddr));

	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(port);
	inet_pton(AF_INET, address, &servaddr.sin_addr);

	if (connect(sockfd, (sockaddr *) &servaddr, sizeof(servaddr)) == -1) {
		perror("Error connecting to host");
                exit(0);
	} else {
		return sockfd;
	}

}

void read_data(int sockfd) {

	ssize_t n;
	char buf[MAXLINE];

	if (recv(sockfd, buf, sizeof buf, n)) {
	    puts(buf);
            sleep(5);
            send(sockfd, "pong!!", 6, 0);
        }

}
