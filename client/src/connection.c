#include "connection.h"

t_config *setup_address(char *port, char *address) {
    t_config *config = malloc(sizeof(t_config));
    memset(&(config->servaddr), 0, sizeof(config->servaddr));
    config->send_singal = 0;
    config->recv_singal = 0;

    config->servaddr.sin_family = AF_INET;
    config->servaddr.sin_addr.s_addr = inet_addr(address);
    config->servaddr.sin_port = htons(atoi(port));

    printf("[CONNECTION]    Set address to %s\n", address);
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
    if(0 != pthread_create(&(config->send_thread), NULL, send_handler, config)) {
        fprintf(stderr, "[CONNECTION ERR]    Failed to create send_thread\n");
    } else {
        printf("[CONNECTION]    Send_thread created\n");
    }
    // if(0 != pthread_create(&(config->recv_thread), NULL, recv_handler, config)) {
    //     fprintf(stderr, "[CONNECTION ERR]    Failed to create recv_thread...\n");
    // } else {
    //     printf("[CONNECTION]    Recv_thread created\n");
    // }
}

void check_args(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: ./uchat_client <port> <address>\n");
        exit(2);
    }
}

void *send_handler(void *ptr) {
    t_config *config = (t_config *)ptr;
    while (1) {
        //continiously check if there is something to send
        while (0 == config->send_singal) {
        }
        //if there is, send and wait for receieve
        if( send(config->sockfd, config->send_buf, strlen(config->send_buf), 0) < 0) {
            fprintf(stderr, "[SEND HANDLER ERR]   Failed to send data.\n");
            exit (3);
        }
        printf("[SEND HANDLER]    Sent: %s\n", config->send_buf);
        //write receieved info to buffer
        if( recv(config->sockfd, config->recv_buf, 2000 , 0) < 0) {
            fprintf(stderr, "[SEND HANDLER ERR]   Failed to receive response to request.\n");
            exit (3);
        }
        config->send_singal = 0;
        printf("[SEND HANDLER]    Receieved: %s\n", config->recv_buf);
    }
}

void *recv_handler(void *ptr) {
    printf ("In recv thread:\n");
    while (1) {
        printf("In recv thread...\n");
        sleep(2);
    }
}
