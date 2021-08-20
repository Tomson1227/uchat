#include "uchat.h"

static void switch_upper_toolbar(GtkBuilder *builder) {
    GtkStack *stack_upper_dialog_toolbar = GTK_STACK(gtk_builder_get_object(builder, "stack_upper_dialog_toolbar"));

    gtk_stack_set_visible_child_name(stack_upper_dialog_toolbar, "chat_topbar_grid");
}

void req_delete_msg(GtkButton *btn, t_chat *chat) {
    GtkWidget *r = gtk_list_box_row_new();
    r = GTK_WIDGET(gtk_list_box_get_selected_row(chat->curr_chat->listbox_msgs));
    t_msg *msg = g_object_get_data(G_OBJECT(r), "msg");
    t_room *room = chat->curr_chat;
    char *temp = send_rq_delete_msg(msg->msg_id);
    enQueue(chat->config->queue_send, temp);

    // switch_upper_toolbar(chat->builder);
    // if (msg) {
    //     gtk_widget_destroy(GTK_WIDGET(msg->row_msg));
    //     gtk_list_box_unselect_all(room->listbox_msgs);
    // }
    // free_msg(msg);
}

void free_msg(t_msg *msg) {
    if (msg) {
        mx_free((void **)&(msg->sender));
        mx_free((void **)&(msg->time));
        mx_free((void **)&(msg->msg));
        mx_free((void **)&(msg));
        msg = NULL;
    }
}

void confirm_delete_msg(t_chat *chat) {
    GtkWidget *r = gtk_list_box_row_new();
    if (gtk_list_box_get_selected_row(chat->curr_chat->listbox_msgs) != NULL) {
        r = GTK_WIDGET(gtk_list_box_get_selected_row(chat->curr_chat->listbox_msgs));
        t_msg *msg = g_object_get_data(G_OBJECT(r), "msg");
        t_room *room = chat->curr_chat;

        switch_upper_toolbar(chat->builder);
        if (msg) {
            gtk_widget_destroy(GTK_WIDGET(msg->row_msg));
            gtk_list_box_unselect_all(room->listbox_msgs);
        }
        free_msg(msg);
    } else 
        printf("no row is selcted\n");
}
