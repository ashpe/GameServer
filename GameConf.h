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

#define SERV_PORT 5552
#define LISTENQ 32
#define MAXLINE 4096
