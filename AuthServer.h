void read_data(int sockfd);
static void * init_thread(void *arg);
addrinfo* getAddrInfo();
struct ThreadParams {
      int connfd;
};

#define LISTENQ 32
#define MAXLINE 4096
