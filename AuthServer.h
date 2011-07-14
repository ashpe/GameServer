//Prototypes
char* check_login(char* login_string);
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
