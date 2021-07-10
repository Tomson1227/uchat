#include "connection.h"

#define CONN_BUFF_SIZE  1024

/*--- Static fuctions declarations ---*/

static void client_connect(t_config *config);
static void launch_threads(t_config *config);
static void *conn_handler(void *ptr);
static void client_send(t_config *config);
static void client_recv(t_config *config, struct pollfd *poll_fd,int timeout);

/*-------------------------------------*/


/*--- Public functions definitions ---*/

void connection_setup(t_config *config) {
        //create queues
    config->queue_send = createQueue();
    config->queue_recv = createQueue();
    client_connect(config);
    launch_threads(config);
}

t_config *address_setup(char *port, char *address) {
    t_config *config = malloc(sizeof(t_config));
    memset(&(config->servaddr), 0, sizeof(config->servaddr));

    config->servaddr.sin_family = AF_INET;
    config->servaddr.sin_addr.s_addr = inet_addr(address);
    printf("[CONNECTION]    Set address to %s\n", address);
    config->servaddr.sin_port = htons(atoi(port));
    printf("[CONNECTION]    Set port to %s\n", port);
    return config;
}

void check_args(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: ./uchat_client <port> <address>\n");
        exit(2);
    }
}

/*-------------------------------------*/


/*--- Static fuctions definitions ---*/

static void client_connect(t_config *config) {
    config->sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (config->sockfd == -1) {
        fprintf(stderr, "[CONNECTION ERR]    Socket creation failed...\n");
        exit(1);
    } else {
        printf("[CONNECTION]    Socket successfully created..\n");
    }
    if (connect(config->sockfd, (struct sockaddr*)&(config->servaddr), sizeof(config->servaddr)) != 0) {
        fprintf(stderr, "[CONNECTION ERR]    Connection with the server failed...\n");
        exit(1);
    }
    else {
        printf("[CONNECTION]    Connected to the server..\n");
    }
}

static void launch_threads(t_config *config) {
    if(0 != pthread_create(&(config->conn_thread), NULL, conn_handler, config)) {
        fprintf(stderr, "[CONNECTION ERR]    Failed to create send_thread\n");
        exit(2);
    } else {
        printf("[CONNECTION]    Conn_thread created\n");
    }
}

static void *conn_handler(void *ptr) {
    t_config *config = (t_config *)ptr;
        //socket timeout handler
    struct pollfd poll_fd;
    poll_fd.fd = config->sockfd;
    poll_fd.events = POLLIN;

    while (1) {
        if (!QueueisEmpty(config->queue_send)) {
                //send and wait for response
            client_send(config);
            client_recv(config, &poll_fd, 3000);
        } else {
                //listen for incoming messages
            client_recv(config, &poll_fd, 100);
        }
    }
}

static void client_send(t_config *config) {
    char *dq = deQueue(config->queue_send);

    if(send(config->sockfd, dq, strlen(dq), 0) < 0) {
        fprintf(stderr, "[SEND ERR]   Failed to send data.\n");
        exit (3);
    }
    printf("[SEND]    Sent: %s\n", dq);
    free(dq);
    dq = NULL;
}

static void client_recv(t_config *config, struct pollfd *poll_fd, int timeout) {
    char temp_buff[CONN_BUFF_SIZE];
    memset(temp_buff, 0, CONN_BUFF_SIZE);

    int ret = poll(poll_fd, 1, timeout);
    switch (ret) {
        case -1:    // Error
            fprintf(stderr, "[RECV ERR]     Poll error\n");
            exit(4);
            break;
        case 0:     // Timeout
            if(timeout > 200) {
                fprintf(stderr, "[RECV ERR]     Poll timeout\n");
            }
            break;
        default:    // get your data
            if(recv(config->sockfd, temp_buff, CONN_BUFF_SIZE , 0) < 0) {
                fprintf(stderr, "[RECV ERR]   Failed to receive response to request.\n");
                exit (3);
            }
            char *enq = (char*)calloc(128, sizeof(char));
            strcpy(enq, temp_buff);
            enQueue(config->queue_recv, enq);
            printf("[RECV]    Receieved: %s\n", enq);
            break;
            }
}

/*-------------------------------------*/
