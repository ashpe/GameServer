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
#include "PacketHandler.h"

using namespace std;

int main(void) {

    char username[USERNAME_LEN];
    char password[PASSWORD_LEN];
   
    puts("**Login**");
    printf("Enter username: ");
    fgets(username, USERNAME_LEN, stdin);
    chomp(username);
    printf("Enter password: ");
    fgets(password, PASSWORD_LEN, stdin);
    chomp(password);
    
    printf("Authing user: %s ..\n", username);

    PacketHandler pck;
    std::string login_string = pck.login(username, password, 1.0);
    
    int auth_sock = connect_to("localhost", AUTH_PORT);
    send(auth_sock, login_string.data(), login_string.length(), 0);

    string auth_code = get_auth_code(auth_sock);
    int serv_sock;

    if (auth_code.length() > 0) {
        puts("Connecting to game server..");
        serv_sock =  connect_to("localhost", SERV_PORT); 
    } else {
        //prompt to login again
        exit(0);
    }

    for( ; ; ) {

        read_data(serv_sock);

    }
}

string get_auth_code(int sockfd) {
    ssize_t n;
    char buf[MAXLINE];
    string auth_code;

    if (recv(sockfd, buf, sizeof buf, n)) {
        Packet::PacketHeader pck;
        pck.ParseFromString(buf);
    
        if (pck.has_auth_code()) {
            puts("Auth successsfull");
            auth_code = pck.auth_code();
        } else {
            puts("Auth failure");
        }
    }

    return auth_code;
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
