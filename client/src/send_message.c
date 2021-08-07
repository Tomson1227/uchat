#include "uchat.h"

int k = 0;

gchar *trim_message(const gchar *buffer) {
    int j = 0;
    int n = strlen(buffer) / 45;
    gchar *output = malloc(sizeof(gchar) * strlen(buffer) + n + 1);

    while (j < strlen(buffer)) {
        if (j % 45 == 0 && j != 0) {
            output[j] = '\n';
            j++;
        }
    else {
            output[j] = buffer[j];
            j++;
        }
    }     
    output[j] = '\0'; 
    return output;
}

t_msg *fill_msg() {
    t_msg *msg = (t_msg *)malloc(sizeof(t_msg));
    //appoint received id
    msg->msg_id = k;
    init_msg(msg);
    return msg;
}

void init_msg(t_msg *msg) {
    msg->row_msg = NULL;
    msg->msg_text = NULL;
    msg->sender = NULL;
    msg->time = NULL;
    msg->msg = NULL;
    msg->msg_type = -1;
    msg->room_id = -1;
}

void free_msg(t_msg *msg) {
    //
    free(msg);
}

char *form_msg(char *sender, char *time, char *text) {
    char *output = malloc(sizeof(char) * (strlen(sender) + strlen(text) + strlen(time) + 3));

    strcpy(output, sender);
    strcat(output, " ");
    strcat(output, time);
    strcat(output, "\n");
    strcat(output, text);
    return output;
}

static void get_time(t_msg *msg) {
    GDateTime   *time;            // for storing current time and date
    gchar       *time_str;        // current time and date as a string
    time     = g_date_time_new_now_local();             // get the current time
    time_str = g_date_time_format(time, "%H:%M:%S");    // convert current time to string
    msg->time = malloc(sizeof(gchar) * strlen(time_str) + 1);
    strcpy(msg->time, time_str);
    g_date_time_unref(time);
}

void add_row_msg(t_room *room, t_msg *msg, char *message, t_chat *chat) {
    GtkWidget *row = gtk_list_box_row_new(); 
    GtkWidget *lbl = gtk_label_new(message);

    msg->msg_text = GTK_LABEL(lbl);
    msg->row_msg = GTK_LIST_BOX_ROW(row);
    gtk_container_add(GTK_CONTAINER(room->listbox_msgs), row);
    gtk_container_add(GTK_CONTAINER(row), lbl);
    if (strcmp(chat->username, msg->sender) == 0)
        gtk_widget_set_halign(lbl, GTK_ALIGN_END);
    else 
        gtk_widget_set_halign(lbl, GTK_ALIGN_START);
    g_object_set_data(G_OBJECT(row), "msg_id", k);
    k++;
    g_object_set_data(G_OBJECT(msg->row_msg), "msg", msg);
    gtk_widget_show(row);
    gtk_widget_show(lbl);
    free(message);
}

void AddListItem(t_chat *chat, const gchar *sText, t_msg *msg, t_room *room) {
    char *message = form_msg(msg->sender, msg->time, sText);
    if (room)
        add_row_msg(room, msg, message, chat);
}

void delete_msg(GtkButton *btn, t_chat *chat) {
    GtkWidget *r = gtk_list_box_row_new();
    r = GTK_WIDGET(gtk_list_box_get_selected_row(chat->curr_chat->listbox_msgs));
    t_msg *msg = g_object_get_data(G_OBJECT(r), "msg");

    if (msg)
        gtk_widget_destroy(GTK_WIDGET(msg->row_msg));
    free_msg(msg);
}

void send_message(GtkEntry *entry, t_chat *chat) {
    t_msg *msg = fill_msg();
    t_room *room = chat->curr_chat; 

    msg->sender = malloc(sizeof(gchar)*strlen(chat->username));
    strcpy(msg->sender, chat->username);
    const gchar *buffer = gtk_entry_get_text(entry);
    get_time(msg);

    if (strlen(buffer) > 0) {
        if (strlen(buffer) > 45) {
            gchar *new_buffer = trim_message(buffer);
            AddListItem(chat, new_buffer, msg, room);
            free(new_buffer);
        }
        else 
            AddListItem(chat, buffer, msg, room);
        gtk_entry_set_text(entry,""); 
    }
    recv_message(-1, -1, "21.40.41", "Hello", chat, "sender");
}

void on_btn_send_message_clicked(GtkButton *btn, t_chat *chat) {
    t_msg *msg = fill_msg();
    GtkEntry *entry = GTK_ENTRY(gtk_builder_get_object(chat->builder, "chat_message_entry"));
    const gchar *buffer = gtk_entry_get_text(entry);
    t_room *room = chat->curr_chat; 

    if (strlen(buffer) > 0) {
        if (strlen(buffer) > 45) {
            gchar *new_buffer = trim_message(buffer);
            AddListItem(chat, new_buffer, msg, room);
            free(new_buffer);
        }
        else 
            AddListItem(chat, buffer, msg, room);
        gtk_entry_set_text(entry,""); 
    }
}
