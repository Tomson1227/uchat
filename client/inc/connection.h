#ifndef CONNECTION_H
#define CONNECTION_H

/* Standard headers */
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>

#include "cJSON.h"

/* Typedefs */
typedef struct s_config {
    struct sockaddr_in servaddr;
    int sockfd;
    pthread_t send_thread;
    pthread_t recv_thread;
    int recv_singal;
    int send_singal;
    char *send_buf;
    char *recv_buf;
}               t_config;

/* Functions */
t_config *setup_address(char *port, char *address);
void client_connect(t_config *config);
void check_args(int argc, char *argv[]);
void launch_threads(t_config *config);
void *send_handler(void *ptr);
void *recv_handler(void *ptr);

#endif /* CONNECTION_H */