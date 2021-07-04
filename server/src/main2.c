#include "uchat_server2.h"

void pipesig_hendler(void)
{
    return;
}

void *thread_socket(void *pointer)
{
    t_socket_list *socket = (t_socket_list *) pointer;
    int poll_request;
    struct pollfd fd = {socket->fd, POLLIN, 0};
    time(&socket->begin);
    
    while(1) {
        if((poll_request = poll(&fd, 1, 10 * 1000)) < 0) {
            close(fd.fd);
            return 0;
        }   
        else if(poll_request > 0) {

        }
        else {
            disconect_socket(socket);
            return 0;
        }
    }
}

void init_server(t_server *server)
{
    if((server->fd = socket(AF_INET , SOCK_STREAM , 0)) < 0) {  
        perror("socket failed");  
        exit(EXIT_FAILURE);  
    }  

    if(setsockopt(server->fd, SOL_SOCKET, SO_REUSEADDR, 
      (char *) &server->option, sizeof(server->option))) {  
        perror("setsockopt");  
        exit(EXIT_FAILURE);
    }  
    
    if(ioctl(server->fd, FIONBIO, (char *) &server->option) < 0) {
        perror("ioctl failed");
        close(server->fd);
        exit(-1);
    }

    server->address.sin_family = AF_INET; 
    server->address.sin_addr.s_addr = INADDR_ANY;  
    server->address.sin_port = htons(PORT);  

    if(bind(server->fd, (struct sockaddr*) &server->address, sizeof(server->address))) {  
        perror("bind failed");  
        exit(EXIT_FAILURE);  
    }

    if(listen(server->fd, MAX_LISTEN_SOCKETS) < 0) {
        perror("listen failed");
        close(server->fd);
        exit(-1);
    }
}

int main(int argc , char *argv[])
{
    t_server server;
    init_server(&server);

    int poll_request;
    struct pollfd fd = {server.fd, POLLIN, 0};
    
	SAVE_CURSOR_POS;

    while(true) {
        if((poll(&fd, 1, 1000)) < 0)
            perror("poll error");
        else if(fd.revents & POLLIN) {
            int new_sd = 0;

            while(new_sd >= 0) {
                printf("TP1\n");

                if((new_sd = accept(server.fd, NULL, NULL)) < 0) {
                    if(errno != EWOULDBLOCK) {
                        perror("accept failed");
                    }

                    break;
                }

                printf("TP2\n");
                pthread_t tid; /* идентификатор потока */
                pthread_attr_t attr; /* атрибуты потока */
                pthread_attr_init(&attr);
                new_socket(&server.socket_head, new_sd);
                pthread_create(&tid, &attr, thread_socket, server.socket_head);
                print_socket_list(server.socket_head);
                printf("TP3\n");
            }
            printf("TP4\n");
        }
        printf("TP5\n");
        print_socket_list(server.socket_head);
    }
}
