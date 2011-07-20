//Includes
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
#include <arpa/inet.h>
#include <string>
#include <iostream>


//class includes
#include "AuthDatabaseHandler.h"

//Prototypes
std::string check_login(std::string username, std::string password, AuthDatabaseHandler dbh);
void read_data(int sockfd, AuthDatabaseHandler dbh);
static void * init_thread(void *arg);
addrinfo* getAddrInfo();
struct ThreadParams {
      int connfd;
};


// Defines

#define LISTENQ 32
#define MAXLINE 4096
