#ifndef UCHAT_SERVER_H
#define UCHAT_SERVER_H //#include "uchat_server.h"

#include <time.h>
#include <poll.h>
#include <stdio.h>

#ifdef __linux__
    #define __USE_XOPEN_EXTENDED
#endif

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
#include "sqlite3.h"

typedef enum s_api {
    LOGIN,
    SIGNUP,
    CREATE_ROOM,
    SND_MSG,
    READ_MSG,
    SEARCH_USER,
    DELETE_ROOM,
    DELETE_MSG,
    EDIT_MSG,
    OLD_DIALOGS
}            t_api;

typedef enum s_rs_status {
    SUCCESS = 0,
    ERROR,
    LOGIN_WRONG_USER,
    LOGIN_WRONG_PASS,
    SIGNUP_USER_EXIST,
    ROOM_DOES_NOT_EXIST
}            t_rs_status;

typedef enum s_msg_type {
    M_MESSAGE,
    M_FILE,
    M_IMAGE
}            t_msg_type;

typedef struct s_create_room {
    int id;
    char customer[40];
}              t_create_room;

typedef struct s_send_message {
    int room_id;
    int message_id;
    char date[20];
}              t_send_message;

typedef struct s_read_message {
    int room_id;
    int message_id;
    int message_type;
    char *message;
    char date[20];
    char sender[40];
    bool update;
}              t_read_message;

typedef struct s_search_user {
    char **users;
}              t_search_user;

typedef struct s_delete_message {
    int id;
}              t_delete_message;

typedef struct s_delete_room {
    int id;
}              t_delete_room;

typedef struct s_edit_message {
    int id;
}              t_edit_message;

typedef struct s_upload_old_dialogs {
    int *id;
    char **dialogs;
}              t_upload_old_dialogs;

typedef struct s_create_message {
    int room_id;
    int message_id;
    char date[20];
}              t_create_message;

typedef union u_union_api {
    t_create_message create_message;
    t_create_room create_room;
    t_send_message send_message;
    t_read_message read_message;
    t_search_user search_user;
    t_delete_message delete_message;
    t_delete_room delete_room;
    t_edit_message edit_message;
    t_upload_old_dialogs upload_old_dialogs;
}             t_union_api;

typedef struct s_message {
    t_api API;
    int status;
    t_union_api Data;
}              t_message;

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
    pthread_t tid;
    struct s_socket_list *next_socket;
    struct s_socket_list *prev_socket;
}              t_socket_list;

typedef struct s_server {
    int fd;
    int port;
    int option;
    sqlite3 *db;
    struct sockaddr_in address;
    t_socket_list *socket_head;
}              t_server;

t_socket_list *new_socket(t_server *server, int fd);
void send_message(char *message);
void sockets_status(t_socket_list *head);
void disconect_socket(t_socket_list *address);
void del_socket_list(t_socket_list **head);

void Init_DB(t_server * server);

void SignUp(t_message *message, char *user_login, char *user_pass);
void LogIn(t_message *message, char *user_login, char *user_pass);
void CreateRoom(t_message *message, char *user, char *customer);
void UserSearch(t_message *message, char *searchText);
void EditMessage(t_message *message, int roomID, char *newMessage);
void SendMessage(t_message *message, char *username, int roomID, char *text, t_msg_type M_MESSAGE);
void ReadMessage(cJSON *rq, int fd);
void DeleteMessage(t_message *message, int messageID);
void DeleteRoom(t_message *message, int roomID);
void UploadOldDialogs(t_message *message, char *username);

void send_response(char* message, int fd);
cJSON *create_response(t_message *message);

//Server function
void process_rq_server(const char *const string, int fd);

#endif /* UCHAT_SERVER_H */
