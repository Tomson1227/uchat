#ifndef UCHAT_SERVER_H
#define UCHAT_SERVER_H //#include "uchat_server.h"

#include <time.h>
#include <poll.h>
#include <stdio.h> 
#include <string.h> 
#include <stdbool.h>
#include <stdlib.h> 
#include <errno.h> 
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <sys/time.h>
#include <sys/ioctl.h>
#include <sys/poll.h>
#include <pthread.h>
#include <fcntl.h>

#include "cJSON.h"
#include "transaction.h"

#define PORT 5000
#define BUFF_SIZE 1024

#define STATUS_CONNECTED "\033[32;1m[CONNECTED]\033[0m"
#define STATUS_DISCONNECTED "\033[31;1m[DISCONNECTED]\033[0m"

#define ESC                 "\033"

/*    C1 set element ANSI escape sequences    */
#define CSI     ESC "["     //Controll Sequence Initiator

/*    SGR(Select Grafical Rendition) parametrs    */
#define SGR_END             "m"

#define BOLD                CSI "1"  SGR_END
#define FAINT               CSI "2"  SGR_END
#define ITALIC              CSI "3"  SGR_END
#define UNDERLINE           CSI "4"  SGR_END
#define INVERT_COLOR        CSI "7"  SGR_END
#define TEXT_HIDE           CSI "8"  SGR_END
#define DEFAULT_FONT        CSI "10" SGR_END

/* FG - ForeGround */
#define FG_COLOR_RED        CSI "31" SGR_END
#define FG_COLOR_GREEN      CSI "32" SGR_END
#define FG_COLOR_ORANGE     CSI "33" SGR_END
#define FG_COLOR_BLUE       CSI "34" SGR_END
#define FG_COLOR_MAGENTA    CSI "35" SGR_END
#define FG_COLOR_CYAN       CSI "36" SGR_END
#define FG_COLOR_LIGHT_GRAY CSI "37" SGR_END
#define FG_COLOR_RESET      CSI "39" SGR_END

/* BG - BackGround */
#define BG_COLOR_BLACK      CSI "40" SGR_END
#define BG_COLOR_RED        CSI "41" SGR_END
#define BG_COLOR_GREEN      CSI "42" SGR_END
#define BG_COLOR_ORANGE     CSI "43" SGR_END
#define BG_COLOR_BLUE       CSI "44" SGR_END
#define BG_COLOR_MAGENTA    CSI "45" SGR_END
#define BG_COLOR_CYAN       CSI "46" SGR_END
#define BG_COLOR_LIGHT_GRAY CSI "47" SGR_END
#define BG_COLOR_RESET      CSI "49" SGR_END

#define RESET_ALL           CSI "0"  SGR_END

#define SAVE_CURSOR_POS     printf(CSI "s")
#define RESTORE_CURSOR_POS  printf(CSI "u")
#define HIDE_CURSOR         printf(CSI "?25l")
#define SHOW_CURSOR         printf(CSI "?25h")

#define ERASE_RIGHT         printf(CSI "0K")
#define ERASE_LEFT          printf(CSI "1K")
#define ERASE_LINE          printf(CSI "2K")

#define ERASE_DOWN          printf(CSI "0J")
#define ERASE_UP            printf(CSI "1J")
#define ERASE_ALL           printf(CSI "2J")

#define CURSOR_NEW_LINE     printf(CSI "1E")
#define CURSOR_PREV_LINE    printf(CSI "1F")

#define MAX_CLIENTS 10
#define MAX_BUFFER  1025
#define MAX_LISTEN_SOCKETS 50

typedef struct s_socket_list {
    int fd;
    bool status;
    time_t begin;
    struct s_socket_list *next_socket;
    struct s_socket_list *prev_socket;
}              t_socket_list;

typedef struct s_server {
    int fd;
    int option;
    struct sockaddr_in address;
    t_socket_list *socket_head;
}              t_server;

t_socket_list *new_socket(t_server *server, int fd);
void sockets_status(t_socket_list *head);
void disconect_socket(t_socket_list *address);
void del_socket_list(t_socket_list **head);
int Init_DB(char *name, const char *structure);

#endif /* UCHAT_SERVER_H */
