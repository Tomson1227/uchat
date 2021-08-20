#ifndef UCHAT_H
#define UCHAT_H

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <time.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netinet/ip.h>
#include <gtk/gtk.h>

#include "connection.h"
#include "queue.h"
#include "API.h"
#include "back.h"

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
    char *username;
    gint username_id;
    gchar *description;
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
    char *chat_name;
    gint room_id;
    bool is_updated;
    char **members;
};

struct s_msg {
    GtkListBoxRow *row_msg;
    GtkLabel *msg_text;
    char *sender;
    char *time;
    char *msg;
    int msg_type;
    int room_id;
    int msg_id;
};

//gui
void load_css(char *path);
void init_chat_window(t_chat *chat, int type);
t_chat *init_chat(t_chat *chat, t_config *config);
void init_gui(t_chat *chat);
gint start_gui(t_chat *chat);
void req_send_message(GtkEntry *entry, t_chat *chat);
void add_message(int id_of_room, int id_of_msg, char *time, t_chat *chat);
void connect_chat(t_chat *chat);
void connect_room_settings(t_chat *chat);
void connect_profile_settings(t_chat *chat);
void connect_search_username_room(t_chat *chat); 
void display_upper_panel(GtkListBox *box, GtkListBoxRow *r, GtkStack *stack_upper_dialog_toolbar); 
void select_room(GtkListBox *box, GtkListBoxRow *row, t_chat *chat);
void display_error_wrong_username_login(t_chat *chat);
void display_error_wrong_password_login(t_chat *chat);
void display_error_user_exists(t_chat *chat);
void create_room(GtkButton *btn, t_chat *chat);
void req_delete_msg(GtkButton *btn, t_chat *chat);
void on_btn_send_message_clicked(GtkButton *btn, t_chat *chat);
void show_room_info(GtkButton *btn, t_chat *chat);
void show_clear_history_window(GtkButton *btn, t_chat *chat);
void show_delete_room_window(GtkButton *btn, t_chat *chat);
void show_block_user_window(GtkButton *btn, t_chat *chat);
void clear_history(GtkButton *btn, t_chat *chat);
void cancel_clear_history(GtkButton *btn, t_chat *chat);
void req_delete_room(GtkButton *btn, t_chat *chat);
void delete_msg(GtkWidget *wdg, t_room *room);
void delete_room(t_room *room, t_chat *chat); 
void cancel_delete_room(GtkButton *btn, t_chat *chat);
void block_user(GtkButton *btn, t_chat *chat);
void cancel_block_user(GtkButton *btn, t_chat *chat); 
void attach_file(GtkButton *btn, t_chat *chat); 
void show_log_out_window(GtkButton *btn, t_chat *chat);
char *form_msg(char *sender, char *time, char *text);
char *trim_message(char *buffer);
t_msg *fill_msg(int room_id, int msg_id, char *time, char *username, char *text);
void free_msg(t_msg *msg);
void add_row_msg(t_room *room, t_msg *msg, char *message, t_chat *chat);
void recv_message(int room_id, int msg_id, gchar *date, gchar *sText, 
                                                     t_chat *chat, gchar *sender, int update);
void AddListItem(t_chat *chat, char *sText, t_msg *msg, t_room *room);                                                     
void log_out(GtkButton *btn, t_chat *chat);
void close_info(GtkButton *btn, t_chat *chat); 
void req_search_user(GtkEntry *entry, t_chat *chat);
void change_password_visibility(GtkEntry *entry);
void filter_search(char **users, int n, t_chat *chat);
void filter_row(GtkWidget *wdg, char **users);
void edit_message(GtkButton *button, t_chat *chat);
void send_edited_message(GtkEntry *entry, t_chat *chat);
void show_create_room_window(GtkButton *btn, t_chat *chat);
void cancel_create_room(GtkButton *btn, t_chat *chat);
void restore_listbox(GtkEntry *entry, GtkListBox *box);
void req_create_dialog(GtkListBox *box, GtkListBoxRow *row, t_chat *chat);
void manage_visibility(GtkListBox *box, t_chat *chat);
void create_dialog(int id, char *customer_string, t_chat *chat);
void upload_old_dialogs(char **dialogs, int *ids, int n, t_chat *chat);
void mx_free(void **ptr);
void set_default_settings(t_room *room, GtkBuilder *builder);
void set_widget_class(GtkWidget *widget, gchar *class_name);
void confirm_delete_msg(t_chat *chat);
void delete_room_confirm(t_chat *chat);
void back_setup(t_chat *chat);
void manage_room_visibility(t_chat *chat);

//api
void process_rs_client(const char *const string, t_chat *chat);
char *send_rq_log_in_client(char *username, char *password);
char *send_rq_sign_in_client(char *username, char *password);
char *send_rq_create_msg_client(gint room_id);
char *send_rq_create_room_client(char *username, char *customer); 
char *send_rq_send_msg_client(char *username, gint room_id, char *message);
char *send_req_old_dialogs(char *username);
char *send_rq_delete_room(int room_id); 
char *send_rq_delete_msg(int msg_id);
char *send_rq_search_username(char *start_of_username);
char *send_req_edit_msg(int msg_id, char *new_text);
char *send_req_upload_messages(int room_id);

//additional functions
char *my_itoa(long long number);
char *mx_strnew(const int size);
char *mx_strdup(const char *str);
char *mx_strcpy(char *dst, const char *src);
#endif /* UCHAT_H */
