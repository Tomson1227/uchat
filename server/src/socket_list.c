#include "uchat_server.h"

static t_socket_list *init_command(void)
{
    t_socket_list *new_socket = NULL;

    if(!(new_socket = (t_socket_list *) calloc(1, sizeof(t_socket_list))))
        strerror(errno);

    new_socket->fd = -1;
    new_socket->begin = time(NULL);
    new_socket->status = false;
    new_socket->next_socket = NULL;
    new_socket->prev_socket = NULL;

    return new_socket;
}

t_socket_list *new_socket(t_server *server, int fd)
{
    t_socket_list *new_socket = init_command();

    new_socket->fd = fd;

    if(!server->socket_head) {
        server->socket_head = new_socket;
        return new_socket;
    }

    t_socket_list *last = server->socket_head;

    while(last->next_socket)
        last = last->next_socket;

    new_socket->prev_socket = last;
    last->next_socket = new_socket;

    return new_socket;
}

void disconect_socket(t_socket_list *socket)
{
	if(!socket)
		return;

    close(socket->fd);
    socket->status = false;
    
    if(socket->next_socket)
        socket->next_socket->prev_socket = socket->prev_socket;

    if(socket->prev_socket)    
        socket->prev_socket->next_socket = socket->next_socket; 

    free(socket);
}

void sockets_status(t_socket_list *head)
{
    int count = 1;
    time_t now = time(NULL);
    RESTORE_CURSOR_POS;
    ERASE_DOWN;

    printf("\033[35;1mSERVER RUN\033[39m: %lds\n\033[0m", now - head->begin);
    head = head->next_socket;

    while(head) {
        printf("SOCKET %d: %4d %4lds %s\n", count++, head->fd, now - head->begin,
               head->status ? STATUS_CONNECTED : STATUS_DISCONNECTED);
                
        head = head->next_socket;
    }
}

void del_socket_list(t_socket_list **head)
{
    t_socket_list *temp;

    while(*head) {
        temp = (*head)->next_socket;
        close((*head)->fd);
        (*head)->status = false;
        free(*head);
        *head = temp;
    }
}

void close_socket_list(t_socket_list *head)
{
    while(head) {
        close(head->fd);
        pthread_join(head->tid, NULL);
        head->status = false;
        head = head->prev_socket;
    }
}
