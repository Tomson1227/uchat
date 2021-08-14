#include "uchat.h"

void delete_msg(GtkButton *btn, t_chat *chat) {
    //send request
    GtkWidget *r = gtk_list_box_row_new();
    r = GTK_WIDGET(gtk_list_box_get_selected_row(chat->curr_chat->listbox_msgs));
    t_msg *msg = g_object_get_data(G_OBJECT(r), "msg");

    if (msg)
        gtk_widget_destroy(GTK_WIDGET(msg->row_msg));
    free_msg(msg);
}
