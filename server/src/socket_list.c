#include "uchat_server2.h"

static t_socket_list *init_command(void)
{
    t_socket_list *new_socket = NULL;

    if(!(new_socket = (t_socket_list *) calloc(1, sizeof(t_socket_list))))
        strerror(errno);

    new_socket->fd = -1;
    new_socket->begin = 0;
    new_socket->status = false;
    new_socket->next_socket = NULL;
    new_socket->prev_socket = NULL;

    return new_socket;
}

void new_socket(t_socket_list **head, int fd)
{
    t_socket_list *new_socket = init_command();
    
    new_socket->fd = fd;

    if(!(*head)) {
        (*head) = new_socket;        
        return;
    }

    new_socket->next_socket = *head;
    (*head)->prev_socket = new_socket;
    (*head) = new_socket;
}

void print_socket_list(t_socket_list *head)
{
    int count = 1;
    time_t now = time(NULL);
    RESTORE_CURSOR_POS;
    ERASE_DOWN;

    while(head) {
        printf("SOCKET %d: %4d %lds %s\n", count++, head->fd, now - head->begin,
               head->status ? STATUS_CONNECTED : STATUS_DISCONNECTED);
        head = head->next_socket;
    }
}

void disconect_socket(t_socket_list *address)
{
	if(!address)
		return;

    close(address->fd);

    if(address->next_socket)
        address->next_socket->prev_socket = address->prev_socket;

    if(address->prev_socket)    
        address->prev_socket->next_socket = address->next_socket; 

    free(address);
    address = NULL;
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
