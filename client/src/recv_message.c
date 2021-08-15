#include "uchat.h"

void recv_message(int room_id, int msg_id, char *date, char *sText, 
                                         t_chat *chat, char *sender) {
    // GObject *stack = gtk_builder_get_object(chat->builder, "stack");
    // if (gtk_stack_get_child_by_name(stack, my_itoa(room_id)) == NULL) {
        // create_dialog(room_id, sender, chat);
        // GtkWidget *scroll = GTK_WIDGET(gtk_stack_get_child_by_name(stack, my_itoa(room_id)));
        // t_room *room = g_object_get_data(scroll, "dialog");
    // }
    // else {
        // GtkWidget *scroll = GTK_WIDGET(gtk_stack_get_child_by_name(stack, my_itoa(room_id)));
        // t_room *room = g_object_get_data(scroll, "dialog");
    // }

    t_room *room = chat->curr_chat; 
    // if (NULL == g_object_get_data(my_itoa(msg_id), "msg_id")) {
        t_msg *msg = fill_msg(room_id, msg_id, date, sender, sText);

       if (strlen(sText) > 45) {
           gchar *new_buffer = trim_message(sText);
           AddListItem(chat, new_buffer, msg, room);
           free(new_buffer);
       }
       else 
           AddListItem(chat, sText, msg, room);
    // }   
}
