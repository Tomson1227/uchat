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
#include <poll.h>

#include "cJSON.h"
#include "queue.h"

/* Typedefs */
typedef struct s_config {
    struct sockaddr_in servaddr;
    int sockfd;
    pthread_t conn_thread;
    queue_t *queue_send;
    queue_t *queue_recv;
}               t_config;

/* Functions */
void connection_setup(t_config *config);
t_config *address_setup(char *port, char *address);
void check_args(int argc, char *argv[]);


#endif /* CONNECTION_H */
