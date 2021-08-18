#include "uchat.h"

int k = 0;

char *trim_message(char *buffer) {
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

t_msg *fill_msg(int room_id, int msg_id, char *time, char *username, char *text) {
    t_msg *msg = (t_msg *)malloc(sizeof(t_msg));

    msg->sender = malloc(sizeof(char)*strlen(username) + 1);
    strcpy(msg->sender, username);
    msg->time = malloc(sizeof(char) * strlen(time) + 1);
    strcpy(msg->time, time);
    // msg->msg_id = k;
    msg->msg_id = msg_id;
    msg->room_id = room_id;
    msg->row_msg = NULL;
    msg->msg_text = NULL;
    msg->msg = malloc(sizeof(char) * strlen(text) + 1);
    strcpy(msg->msg, text);
    msg->msg_type = -1;
    return msg;
}

char *form_msg(char *sender, char *time, char *text) {
    char *output = malloc(sizeof(char) * (strlen(sender) + strlen(text) + strlen(time) + 3));//strnew

    strcpy(output, sender);
    strcat(output, " ");
    strcat(output, time);
    strcat(output, "\n");
    strcat(output, text);
    return output;
}

void add_row_msg(t_room *room, t_msg *msg, char *message, t_chat *chat) {
    printf("entered addrowmsg function");
    GtkWidget *row = gtk_list_box_row_new(); 
    GtkWidget *lbl = gtk_label_new(message);

    msg->msg_text = GTK_LABEL(lbl);
    msg->row_msg = GTK_LIST_BOX_ROW(row);
    gtk_container_add(GTK_CONTAINER(room->listbox_msgs), row);
    gtk_container_add(GTK_CONTAINER(row), lbl);
    if (strcmp(chat->username, msg->sender) == 0) {
        gtk_widget_set_halign(lbl, GTK_ALIGN_END);
        set_widget_class(lbl, "message_own");
    }
    else {
        gtk_widget_set_halign(lbl, GTK_ALIGN_START);
        set_widget_class(lbl, "message_received");
    } 
    g_object_set_data(G_OBJECT(msg->row_msg), "msg", msg);
    char *id = my_itoa(msg->msg_id);
    g_object_set_data(G_OBJECT(row), "msg_id", id);
    // k++;//
    gtk_widget_show(row);
    gtk_widget_show(lbl);

    free(message);
}

void AddListItem(t_chat *chat, char *sText, t_msg *msg, t_room *room) {
    printf("entered addlistitem function");
    char *message = form_msg(msg->sender, msg->time, sText);
    if (room)
        add_row_msg(room, msg, message, chat);
}

void add_message(int id_of_room, int id_of_msg, char *time, t_chat *chat) {
    printf("entered add message function");
    GtkEntry *entry = GTK_ENTRY(gtk_builder_get_object(chat->builder, "chat_message_entry"));
    char *buffer = NULL;
    buffer = malloc(sizeof(char) * gtk_entry_get_text_length(entry) + 1);
    sprintf(buffer, "%s", gtk_entry_get_text(entry));
    // t_msg *msg = fill_msg(chat->curr_chat->room_id, k, time, chat->username, buffer);
    t_msg *msg = fill_msg(chat->curr_chat->room_id, id_of_msg, time, chat->username, buffer);
    t_room *room = chat->curr_chat;

    if (strlen(buffer) > 0) {
        if (strlen(buffer) > 45) {
            gchar *new_buffer = trim_message(buffer);
            AddListItem(chat, new_buffer, msg, room);
        }
        else 
            AddListItem(chat, buffer, msg, room);
        gtk_entry_set_text(entry,""); 
    }
    // recv_message(-1, -1, "21.40.41", "Hello", chat, "sender");
}

void req_send_message(GtkEntry *entry, t_chat *chat) {
    char *buffer = NULL;
    buffer = malloc(sizeof(char) * gtk_entry_get_text_length(entry) + 1);
    sprintf(buffer, "%s", gtk_entry_get_text(entry));
    char *tmp = send_rq_send_msg_client(chat->username, chat->curr_chat->room_id, buffer);
    enQueue(chat->config->queue_send, tmp);
    // t_room *room = chat->curr_chat;
    // add_message(room->room_id, k, "21:78:32", chat);
}

void on_btn_send_message_clicked(GtkButton *btn, t_chat *chat) {
    GtkEntry *entry = GTK_ENTRY(gtk_builder_get_object(chat->builder, "chat_message_entry"));
    char *buffer = NULL;
    buffer = malloc(sizeof(char) * gtk_entry_get_text_length(entry) + 1);
    sprintf(buffer, "%s", gtk_entry_get_text(entry));
    char *tmp = send_rq_send_msg_client(chat->username, chat->curr_chat->room_id, buffer);
    enQueue(chat->config->queue_send, tmp);
}
