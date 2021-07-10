#include "connection.h"

#define CONN_BUFF_SIZE      1024
#define RECONNECT_TIMEOUT   3

/*--- Static fuctions declarations ---*/

static void client_connect(t_config *config);
static void launch_threads(t_config *config);
static void *conn_handler(void *ptr);
static void client_send(t_config *config);
static void client_recv(t_config *config, int timeout);
static void client_check_connection(t_config *config);
static void client_reconnect(t_config *config);

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
        //try to create socket
    while(-1 == (config->sockfd = socket(AF_INET, SOCK_STREAM, 0))) {
        fprintf(stderr, "[CONNECTION ERR]    Socket creation failed!\n");
        fprintf(stderr, "[CONNECTION ERR]    Try again in %d seconds...\n", RECONNECT_TIMEOUT); 
        sleep(RECONNECT_TIMEOUT);
    }
    printf("[CONNECTION]    Socket successfully created..\n");

    while(0 != connect(config->sockfd, (struct sockaddr*)&(config->servaddr), sizeof(config->servaddr))) {
        fprintf(stderr, "[CONNECTION ERR]    Connection with the server failed!\n");
        fprintf(stderr, "[CONNECTION ERR]    Try again in %d seconds...\n", RECONNECT_TIMEOUT);
        sleep(RECONNECT_TIMEOUT);
    }
    printf("[CONNECTION]    Connected to the server..\n");
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

    while (1) {
        if (!QueueisEmpty(config->queue_send)) {
                //send and wait for response
            client_send(config);
            client_recv(config, 3000);
        } else {
                //listen for incoming messages
            client_recv(config, 100);
        }
    }
}

static void client_send(t_config *config) {

        //socket timeout handler
    struct pollfd poll_fd;
    poll_fd.fd = config->sockfd;
    poll_fd.events = POLLOUT;

    int ret = poll(&poll_fd, 1, 1000);
    if(poll_fd.revents & (POLLERR | POLLHUP)) {
        printf("[SEND ERR] Disconnect\n");
        client_reconnect(config);
        return;
    }
        //handle revents
    if(poll_fd.revents & POLLOUT) {
        char *dq = deQueue(config->queue_send);

        if(send(config->sockfd, dq, strlen(dq), 0) < 0) {
            fprintf(stderr, "[SEND ERR]   Failed to send data.\n");
            exit (3);
        }
        printf("[SEND]    Sent: %s\n", dq);
        free(dq);
        dq = NULL;
    }
}

static void client_recv(t_config *config, int timeout) {
            //socket timeout handler
    struct pollfd poll_fd;
    poll_fd.fd = config->sockfd;
    poll_fd.events = POLLIN;

    char temp_buff[CONN_BUFF_SIZE];
    memset(temp_buff, 0, CONN_BUFF_SIZE);

    int ret = poll(&poll_fd, 1, timeout);
        //handle revents
    if(poll_fd.revents & (POLLERR | POLLHUP)) {
        printf("[RECV ERR] Disconnect\n");
        client_reconnect(config);
        return;
    }
    if(poll_fd.revents & POLLIN) {
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
                if(recv(config->sockfd, temp_buff, CONN_BUFF_SIZE , 0) <= 0) {
                    fprintf(stderr, "[RECV ERR]   Check connection.\n");
                        //check if connection is ok
                    client_check_connection(config);
                    return;
                } else {
                    char *enq = (char*)calloc(128, sizeof(char));
                    strcpy(enq, temp_buff);
                    enQueue(config->queue_recv, enq);
                    printf("[RECV]    Receieved: %s\n", enq);
                    break;
                }
        }
    }
}

static void client_check_connection(t_config *config) {
    //check connection state
        //socket timeout handler
    struct pollfd poll_fd;
    poll_fd.fd = config->sockfd;
    poll_fd.events = POLLOUT | POLLIN;
    char *test = "TEST";

    int ret = poll(&poll_fd, 1, 1000);
    if(poll_fd.revents & (POLLERR | POLLHUP)) {
        printf("[RECONNECT] Disconnect\n");
        client_reconnect(config);
        return;
    }

    if(poll_fd.revents & POLLIN) {
        printf("[RECONNECT] Ready to receieve\n");
    }
    if(poll_fd.revents & POLLOUT) {
        printf("[RECONNECT] Ready to send\n");
        int snd = send(config->sockfd, test, strlen(test), 0);
        printf("[RECONNECT] Send returned %d\n", snd);
    }
}

static void client_reconnect(t_config *config) {
    close(config->sockfd);
    printf("[RECONNECT] Trying to reconnect...\n");
    client_connect(config);
}

/*-------------------------------------*/
