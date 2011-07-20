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

using namespace std;

int main(void) {

    char username[USERNAME_LEN];
    char password[PASSWORD_LEN];
    char auth_string[AUTH_LEN];
    puts("**Login**");
    printf("Enter username: ");
    fgets(username, USERNAME_LEN, stdin);
    chomp(username);
    printf("Enter password: ");
    fgets(password, PASSWORD_LEN, stdin);
    chomp(password);
    snprintf(auth_string, AUTH_LEN, "LOGIN\nUSER:%s\nPASS:%s", username, password);
    int sock = connect_to("localhost", AUTH_PORT);
    send(sock, auth_string, AUTH_LEN, 0);
    for( ; ; ) {

        read_data(sock);

    }
}

void chomp(char *string) {
    char* p = strchr(string, '\n');
    if (p)
        *p = '\0';
}

int connect_to(const char* address, int port) {

    int		   sockfd;
    sockaddr_in        servaddr;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    bzero(&servaddr, sizeof(servaddr));

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
    string resp;

    if (recv(sockfd, buf, sizeof buf, n)) {
        puts(buf);
        if (strstr(buf,"LOGIN")) {
            puts("Logged in Successfully");
            resp = "L_OK";
        } else if (strcmp(buf,"DIE") == 0) {
            puts("Incorrect login, bai");
            exit(0);
        } else if (strcmp(buf,"PING") == 0) {
            sleep(5);
            resp = "PONG";
        }

        bzero(buf, sizeof(buf));

        send(sockfd, resp.c_str(), resp.length(), 0);
    }

}
