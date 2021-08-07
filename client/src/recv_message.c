#include "uchat.h"

void recv_message(int room_id, int msg_id, const gchar *date, const gchar *sText, 
                                                     t_chat *chat, gchar *sender) {
    // GObject *stack = gtk_builder_get_object(chat->builder, "stack");
    // GtkWidget *scroll = GTK_WIDGET(gtk_stack_get_child_by_name(stack, my_itoa(room_id)));
    // t_room *room = g_object_get_data(scroll, "dialog");
    t_room *room = chat->curr_chat; 
    t_msg *msg = fill_msg();
    msg->time = malloc(sizeof(gchar)*strlen(date) + 1);
    strcpy(msg->time, date);

    msg->sender = malloc(sizeof(gchar)*strlen(sender) + 1);
    strcpy(msg->sender, sender);
    if (strlen(sText) > 45) {
        gchar *new_buffer = trim_message(sText);
        AddListItem(chat, new_buffer, msg, room);
        free(new_buffer);
    }
    else 
        AddListItem(chat, sText, msg, room);
}
