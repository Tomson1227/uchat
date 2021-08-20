#include "uchat.h"

static bool row_is_already_uploaded(GtkListBox *box, int msg_id) {
    bool result = false;
    GList *rows = NULL;
    GList *listrunner;
    t_msg *msg = NULL;

    if (gtk_container_get_children(GTK_CONTAINER(box)) != NULL) {
        rows = gtk_container_get_children(GTK_CONTAINER(box));
        listrunner = g_list_first(rows);    
        while (listrunner) {
            msg = g_object_get_data(G_OBJECT(listrunner->data), "msg");
            if (msg->msg_id == msg_id) {
                result = true; 
            }
            listrunner = g_list_next(listrunner);
        }
    }
    return result;
}

void recv_message(int room_id, int msg_id, char *date, char *sText, 
                            t_chat *chat, char *sender, int update) {
    t_room *room = chat->curr_chat;
    
    if (!row_is_already_uploaded(room->listbox_msgs, msg_id) || update != 0) {
        t_msg *msg = fill_msg(room_id, msg_id, date, sender, sText);

        if (strlen(sText) > 45) {
            char *new_buffer = trim_message(sText);
            AddListItem(chat, new_buffer, msg, room);
        }
        else 
            AddListItem(chat, sText, msg, room);
    }   
}
