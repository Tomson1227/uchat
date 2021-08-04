#include "uchat.h"

int k = 0;
static void free_msg(t_msg *msg);
static gchar *trim_message(gchar *s);
static void init_msg(t_msg *msg);
static t_msg *fill_msg(); 

static gchar *trim_message(gchar *buffer) {
    int j = 0;
    int n = strlen(buffer) / 45;
    gchar *output = malloc(sizeof(gchar) * strlen(buffer) + n + 1);

    while (j < strlen(buffer)) {
        if (j % 45 == 0) {
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

static t_msg *fill_msg() {
    t_msg *msg = (t_msg *)malloc(sizeof(t_msg));
    msg->msg_id = k;
    init_msg(msg);
    return msg;
}

static void init_msg(t_msg *msg) {
    msg->row_msg = NULL;
    msg->row_time = NULL;
    msg->msg_text = NULL;
    msg->msg_date = NULL;
    msg->msg_type = -1;
    msg->login = NULL;
    msg->room_id = -1;
}

static void free_msg(t_msg *msg) {
    free(msg);
}

static void *get_time(t_chat *chat, t_msg *msg) {
    GDateTime   *time;            // for storing current time and date
    gchar       *time_str;        // current time and date as a string
    time     = g_date_time_new_now_local();             // get the current time
    time_str = g_date_time_format(time, "%H:%M:%S");    // convert current time to string
    GtkWidget *row = gtk_list_box_row_new(); 
    GtkWidget *lbl = gtk_label_new(time_str);
    t_room *room = chat->curr_chat;

    msg->msg_date = GTK_LABEL(lbl);
    msg->row_time = GTK_LIST_BOX_ROW(row);
    gtk_container_add(GTK_CONTAINER(room->listbox_msgs), row);
    gtk_container_add(GTK_CONTAINER(row), lbl);
    gtk_list_box_row_set_selectable(GTK_LIST_BOX_ROW(row), FALSE);  
    gtk_label_set_selectable(GTK_LABEL(lbl), FALSE);
    gtk_widget_set_halign(row, GTK_ALIGN_END);
    gtk_widget_show(row);     
    gtk_widget_show(lbl);
    g_free(time_str);
    g_date_time_unref(time);
}

static void AddListItem(t_chat *chat, const gchar *sText, t_msg *msg) {
    // GObject *stack = gtk_builder_get_object(chat->builder, "stack");
    // gchar *s = gtk_stack_get_visible_child_name(GTK_STACK(stack));
    // GObject *scroll = gtk_stack_get_child_by_name(stack, s);
    // t_room *room = g_object_get_data(scroll, "dialog");
    t_room *room = chat->curr_chat;  
    if (room) {
        GtkWidget *row = gtk_list_box_row_new(); 
        GtkWidget *lbl = gtk_label_new(sText);
        msg->msg_text = GTK_LABEL(lbl);
        msg->row_msg = GTK_LIST_BOX_ROW(row);

        gtk_container_add(GTK_CONTAINER(room->listbox_msgs), row);
        gtk_container_add(GTK_CONTAINER(row), lbl);
        gtk_widget_set_halign(lbl, GTK_ALIGN_END);
        g_object_set_data(row, "msg_id", k);
        k++;
        g_object_set_data(msg->row_msg, "msg", msg);
        gtk_widget_show(row);
        gtk_widget_show(lbl);
    }  
}

void delete_msg(GtkButton *btn, t_chat *chat) {
    GtkListBoxRow *r = gtk_list_box_row_new();
    r = gtk_list_box_get_selected_row(chat->curr_chat->listbox_msgs);
    t_msg *msg = g_object_get_data(r, "msg");

    //send request to delete message

    if (msg) {
        gtk_widget_destroy(msg->row_msg);
        gtk_widget_destroy(msg->row_time);
    }
    //free msg structure
    free_msg(msg);
}

void send_message(GtkEntry *entry, t_chat *chat) {
    t_msg *msg = fill_msg();
    const gchar *buffer = gtk_entry_get_text(entry);

    if (strlen(buffer) > 0) {
        if (strlen(buffer) > 45) {
            gchar *new_buffer = trim_message(buffer);
            AddListItem(chat, new_buffer, msg);
            free(new_buffer);
        }
        else 
            AddListItem(chat, buffer, msg);
        get_time(chat, msg);
        gtk_entry_set_text(entry,""); 
    }
}

void on_btn_send_message_clicked(GtkButton *btn, t_chat *chat) {
    t_msg *msg = fill_msg();
    GtkEntry *entry = GTK_ENTRY(gtk_builder_get_object(chat->builder, "chat_message_entry"));
    const gchar *buffer = gtk_entry_get_text(entry);

    if (strlen(buffer) > 0) {
        if (strlen(buffer) > 45) {
            gchar *new_buffer = trim_message(buffer);
            AddListItem(chat, new_buffer, msg);
            free(new_buffer);
        }
        else 
            AddListItem(chat, buffer, msg);
        get_time(chat, msg);
        gtk_entry_set_text(entry,""); 
    }
}
