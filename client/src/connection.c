#include "connection.h"

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

void client_connect(t_config *config) {
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

void launch_threads(t_config *config) {

    if(0 != pthread_create(&(config->conn_thread), NULL, conn_handler, config)) {
        fprintf(stderr, "[CONNECTION ERR]    Failed to create send_thread\n");
        exit(6);
    } else {
        printf("[CONNECTION]    Send_thread created\n");
    }
}

void check_args(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: ./uchat_client <port> <address>\n");
        exit(2);
    }
}


void *conn_handler(void *ptr) {
    t_config *config = (t_config *)ptr;
        //socket timeout handler
    struct pollfd poll_fd;
    int ret;
    poll_fd.fd = config->sockfd;
    poll_fd.events = POLLIN;

    while (1) {
        if (!QueueisEmpty(config->queue_send)) {
            char *dq = deQueue(config->queue_send);

            if( send(config->sockfd, dq, strlen(dq), 0) < 0) {
                fprintf(stderr, "[SEND HANDLER ERR]   Failed to send data.\n");
                exit (3);
            }
            printf("[SEND HANDLER]    Sent: %s\n", dq);
            free(dq);
                //write receieved info to buffer
            char temp_buff[128];
            //handle receive timeout
            ret = poll(&poll_fd, 1, 3000); //timeout
            switch (ret) {
            case -1:    // Error
                fprintf(stderr, "[SEND HANDLER ERR]     Poll error\n");
                exit(4);
                break;
            case 0:     // Timeout
                fprintf(stderr, "[SEND HANDLER ERR]     Poll timeout\n");
                break;
            default:    // get your data
                if( recv(config->sockfd, temp_buff, 2000 , 0) < 0) {
                    fprintf(stderr, "[SEND HANDLER ERR]   Failed to receive response to request.\n");
                    exit (3);
                }
                char *enq = (char*)calloc(128, sizeof(char));
                strcpy(enq, temp_buff);
                memset(temp_buff, 0, 128);
                enQueue(config->queue_recv, enq);
                printf("[SEND HANDLER]    Receieved: %s\n", enq);
                break;
            }
        } else {
            //check for incoming message
            char temp_buff[128];
                        //handle receive timeout
        ret = poll(&poll_fd, 1, 100); //timeout
        switch (ret) {
        case -1:    // Error
            fprintf(stderr, "[RECV HANDLER ERR]     Poll error\n");
            exit(4);
            break;
        case 0:     // Timeout
            fprintf(stderr, "[RECV HANDLER ERR]     Poll timeout\n");
            break;
        default:    // get your data
            if( recv(config->sockfd, temp_buff, 200 , 0) <= 0) {
                fprintf(stderr, "[RECV HANDLER ERR]   Failed to receive response to request.\n");
                exit (3);
            }
            char *enq = (char*)calloc(128, sizeof(char));
            strcpy(enq, temp_buff);
            memset(temp_buff, 0, 128);
            enQueue(config->queue_recv, enq);
            printf("[RECV HANDLER]    Receieved: %s\n", enq);
            break;
            }
        }
    }
}
