#include "uchat_server2.h"

bool SetSocketBlockingEnabled(int fd, bool blocking)
{
   if (fd < 0) return false;

#ifdef _WIN32
   unsigned long mode = blocking ? 0 : 1;
   return (ioctlsocket(fd, FIONBIO, &mode) == 0) ? true : false;
#else
   int flags = fcntl(fd, F_GETFL, 0);
   if (flags == -1) return false;
   flags = blocking ? (flags & ~O_NONBLOCK) : (flags | O_NONBLOCK);
   return (fcntl(fd, F_SETFL, flags) == 0) ? true : false;
#endif
}

void *thread_socket(void *pointer)
{
    t_socket_list *socket = (t_socket_list *) pointer;
    char *request;
    int poll_request;
    struct pollfd fd = {socket->fd, POLLIN, 0};
    socket->status = true;
    time(&socket->begin);
    send(socket->fd, "Server connected", 17, 0);

    while(1) {
        if((poll_request = poll(&fd, 1, 10 * 1000)) > 0) {
            if(!(request = read_socket(socket->fd)))
                break;     
            
            process_request(request, socket->fd);

            free(request);
        }
        else
            break;
    }

    disconect_socket(socket);
    return NULL;
}

void init_server(t_server *server)
{
    server->socket_head = NULL;

    if((server->fd = socket(AF_INET , SOCK_STREAM , 0)) < 0) {  
        perror("socket failed");  
        exit(EXIT_FAILURE);  
    }  

    if(setsockopt(server->fd, SOL_SOCKET, SO_REUSEADDR, 
      (char *) &server->option, sizeof(server->option))) {  
        perror("setsockopt");  
        exit(EXIT_FAILURE);
    }  
    
    SetSocketBlockingEnabled(server->fd, true);

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

    SetSocketBlockingEnabled(server->fd, false);

    if(listen(server->fd, MAX_LISTEN_SOCKETS) < 0) {
        perror("listen failed");
        close(server->fd);
        exit(-1);
    }

    new_socket(server, server->fd);
    server->socket_head->status = true;
}

int main(int argc , char *argv[])
{
    t_server server;
    init_server(&server);

    int poll_request;
    struct pollfd fd = {server.fd, POLLIN, 0};
    
	SAVE_CURSOR_POS;

    while(true) {
        if((poll(&fd, 1, 200)) < 0)
            perror("poll error");
        else if(fd.revents & POLLIN) {
            int new_sd = 0;

            while(new_sd >= 0) {
                if((new_sd = accept(server.fd, NULL, NULL)) < 0) {
                    if(errno != EWOULDBLOCK) {
                        perror("accept failed");
                    }

                    break;
                }

                pthread_t tid; /* идентификатор потока */
                pthread_attr_t attr; /* атрибуты потока */
                pthread_attr_init(&attr);
                pthread_create(&tid, &attr, thread_socket, new_socket(&server, new_sd));
            }
        }

        sockets_status(server.socket_head);
    }
}
