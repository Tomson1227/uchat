#pragma once

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>
#include <stdlib.h>
#include "connection.h"
#include "queue.h"
#include "API.h"
#include <arpa/inet.h>
#include <netinet/ip.h>
#include <gtk/gtk.h>
#include "connection.h"

typedef struct s_msg t_msg;
typedef struct s_room t_room;
typedef struct s_chat t_chat;

struct s_chat{
    GtkBuilder *builder;
    GtkStack *stack_dialog_box;
    GtkStack *stack_upper_dialog_toolbar;
    GtkScrolledWindow *scrll_wndw_dlgs;
    GtkWidget *label_msg;
    GtkWidget *row;
    gchar *username;
    GtkLabel *lbl_slct_chat;
    GtkListBoxRow *row_slctd;
    GtkListBox *listbox_dlgs;
    t_room *curr_chat;
    t_config *config;
};

struct s_room {
    GtkStack *stack;
    GtkScrolledWindow *scrll_wndw_msgs;
    GtkViewport *vwprt;
    GtkListBox *listbox_msgs;
    GtkListBoxRow *row_chat;
    GtkListBoxRow *row_msg;
    gint row_index;
    GtkLabel *lbl;
    GtkLabel *chat_name;
    gint room_id;
    GHashTable *members;
};

struct s_msg {
    GtkListBoxRow *row_msg;
    GtkListBoxRow *row_time;
    GtkLabel *msg_text;
    GtkLabel *msg_date;
    gint msg_type;
    gchar *login;
    gint room_id;
    gint msg_id;
};

//gui
void load_css(char *path);
void init_chat_window(t_chat *chat);
t_chat *init_chat(t_chat *chat, t_config *config);
void init_gui(t_chat *chat);
gint start_gui(t_chat *chat);
void connect_chat(t_chat *chat);
void send_message(GtkButton *btn, t_chat *chat);

void mx_display_upper_panel(GtkListBox *box, GtkListBoxRow *r, GtkStack *stack_upper_dialog_toolbar); 
void select_room(GtkListBox *box, GtkListBoxRow *row, t_chat *chat);
void display_error_wrong_username_login(t_chat *chat);
void display_error_wrong_password_login(t_chat *chat);
void display_error_user_exists(t_chat *chat);
void delete_room(t_room *room);
void create_room(GtkButton *btn, t_chat *chat);
// void delete_msg(GtkButton *btn, t_chat *chat);
//api
char *send_rq_log_in_client(char *username, char *password);
char *send_rq_sign_in_client(char *username, char *password);
void process_rs_client(const char *const string, t_chat *chat);

//additional functions
char *my_itoa(long long number);

