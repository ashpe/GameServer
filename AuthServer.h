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

//Prototypes
std::string check_login(char *login_string);
void read_data(int sockfd);
static void * init_thread(void *arg);
addrinfo* getAddrInfo();
struct ThreadParams {
      int connfd;
};

// Includes

#include <sqlite3.h>

// Defines

#define LISTENQ 32
#define MAXLINE 4096
