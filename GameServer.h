void onSockRead(int sockfd);
static void * init_thread(void *arg);
addrinfo* getAddrInfo();
struct ThreadParams {
      int connfd;
};
