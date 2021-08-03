#include "uchat.h"

int k = 0;

static t_msg *fill_msg(char *rs);
static void init_msg(t_msg *msg);

static t_msg *fill_msg(char *rs) {
    t_msg *msg = malloc(sizeof(msg));
    msg->msg_id = k;
    k++; 
    init_msg(msg);
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

static void *get_time(t_room *room, t_msg *msg) {
    GDateTime   *time;            // for storing current time and date
    gchar       *time_str;        // current time and date as a string
    time     = g_date_time_new_now_local();             // get the current time
    time_str = g_date_time_format(time, "%H:%M:%S");    // convert current time to string
    GtkWidget *row = gtk_list_box_row_new(); 
    GtkWidget *lbl = gtk_label_new(time_str);

    msg->msg_date = GTK_LABEL(lbl);
    msg->row_time = GTK_LIST_BOX_ROW(row);
    gtk_container_add(GTK_CONTAINER(room->listbox_msgs), row);
    gtk_container_add(GTK_CONTAINER(row), lbl);
    gtk_list_box_row_set_selectable(GTK_LIST_BOX_ROW(row), FALSE);  
    gtk_label_set_selectable(GTK_LABEL(lbl), FALSE);
    gtk_widget_set_halign(row, GTK_ALIGN_END);
    gtk_widget_show(row);     
    gtk_widget_show(lbl);
    // g_free(time_str);
    // g_date_time_unref(time);
}

static void AddListItem(t_chat *chat, char *sText, t_msg *msg) {
    GObject *stack = gtk_builder_get_object(chat->builder, "stack");
    const gchar *s = gtk_stack_get_visible_child_name(GTK_STACK(stack));
    GtkScrolledWindow *scroll = gtk_stack_get_child_by_name(stack, s);
    t_room *room = g_object_get_data(scroll, "dialog");    
    GtkWidget *row = gtk_list_box_row_new(); 
    GtkWidget *lbl = gtk_label_new(sText);
    msg->msg_text = GTK_LABEL(lbl);
    msg->row_msg = GTK_LIST_BOX_ROW(row);

    gtk_container_add(GTK_CONTAINER(room->listbox_msgs), row);
    gtk_container_add(GTK_CONTAINER(row), lbl);
    gtk_widget_set_halign(lbl, GTK_ALIGN_END);
    g_object_set_data(row, "msg_id", msg->msg_id);
    g_object_set_data(msg->row_msg, "msg", msg);
    gtk_widget_show(row);
    gtk_widget_show(lbl);
    get_time(room, msg);
}

void send_message(GtkButton *btn, t_chat *chat) {
    t_msg *msg = fill_msg(NULL);
    char buffer[1024];
    GtkEntry *entry = GTK_ENTRY(gtk_builder_get_object(chat->builder, "chat_message_entry"));

    sprintf(buffer, "%s", gtk_entry_get_text(entry));

    if (strlen(buffer) > 0) {
        //inserting '\n' into the message after each 45 symbols
        if (strlen(buffer) > 45) {
            char new_buffer[strlen(buffer) + (strlen(buffer) % 45)];
            int i = 0;

            while (i < strlen(buffer)) {
                if (i % 45 == 0) {
                    new_buffer[i] = '\n';
                    i++;
                }
                else {
                    new_buffer[i] = buffer[i];
                    i++;
                }
            }              
            AddListItem(chat, new_buffer, msg);    
        } else {
            AddListItem(chat, buffer, msg);
        }
        gtk_entry_set_text(entry,""); 
    }
}
