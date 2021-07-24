#include "uchat_server.h"

static void *thread_socket(void *pointer);
static inline void init_server(t_server *server);

int main(int argc , char *argv[])
{
    t_server server;
    init_server(&server);
    Init_DB(&server);

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

static bool SetSocketBlockingEnabled(int fd, bool blocking)
{
    if(fd < 0) 
        return false;
    
    int flags = fcntl(fd, F_GETFL, 0);
    
    if(flags == -1) 
        return false;

    flags = blocking ? (flags & ~O_NONBLOCK) : (flags | O_NONBLOCK);
    
    return !fcntl(fd, F_SETFL, flags);
}

static char *receive_line(int fd)
{
    char *line =  NULL;
    char buff[BUFF_SIZE];
    ssize_t size;

    while(1) {
        size = recv(fd, buff, BUFF_SIZE, 0);

        if(size > 0 || size < BUFF_SIZE) {
            return strcpy(line, buff);
        }
        else if(errno != EWOULDBLOCK) {
            if(!line)
                free(line);

            return NULL;
        }
    }

    return line;
}

static void *thread_socket(void *pointer)
{
    t_socket_list *socket = (t_socket_list *) pointer;
    char *request;
    int poll_request;
    struct pollfd fd = {socket->fd, POLLIN, 0};
    socket->status = true;
    time(&socket->begin);
    // send(socket->fd, "Server connected\n", 18, 0);

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

static inline void init_server(t_server *server)
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
