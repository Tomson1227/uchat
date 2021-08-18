#include "uchat.h"

void req_delete_room(GtkButton *btn, t_chat *chat) {
    t_room *room = chat->curr_chat;
    //char *tmp = send_rq_delete_room(chat->curr_chat->room_id);
    //enQueue(chat->config->queue_send, tmp);
    gtk_container_foreach(GTK_CONTAINER(chat->curr_chat->listbox_msgs), (GtkCallback)delete_msg, chat->curr_chat);
    delete_room(room, chat);
}

void delete_msg(GtkWidget *wdg, t_room *room) {
    t_msg *msg = g_object_get_data(G_OBJECT(wdg), "msg");

    if (msg) {
        gtk_widget_destroy(GTK_WIDGET(msg->row_msg));
        gtk_list_box_unselect_all(room->listbox_msgs);
    }
    free_msg(msg);
}

void set_default_settings(t_room *room, GtkBuilder *builder) {
    GObject *delete_room = gtk_builder_get_object(builder, "delete_room");
    GObject *wndw_room_info = gtk_builder_get_object(builder, "wndw_room_info");
    GtkBox *bottom_bar_chat = GTK_BOX(gtk_builder_get_object(builder, "bottom_bar_chat"));
    GtkStack *stack_upper_dialog_toolbar = GTK_STACK(gtk_builder_get_object(builder, "stack_upper_dialog_toolbar"));
    GtkGrid *chat_topbar_grid = GTK_GRID(gtk_builder_get_object(builder, "chat_topbar_grid"));
    GtkGrid *chat_topbar_wth_btns = GTK_GRID(gtk_builder_get_object(builder, "chat_topbar_wth_btns"));
    
    gtk_widget_set_visible(GTK_WIDGET(delete_room), FALSE);
    gtk_widget_set_visible(GTK_WIDGET(wndw_room_info), FALSE);
    gtk_widget_set_visible(GTK_WIDGET(bottom_bar_chat), FALSE);
    gtk_widget_set_visible(GTK_WIDGET(stack_upper_dialog_toolbar), FALSE);
    gtk_widget_set_visible(GTK_WIDGET(chat_topbar_grid), FALSE);
    gtk_widget_set_visible(GTK_WIDGET(chat_topbar_wth_btns), FALSE);
    gtk_widget_set_visible(GTK_WIDGET(room->row_chat), FALSE);
    gtk_widget_set_visible(GTK_WIDGET(room->scrll_wndw_msgs), FALSE);
}

void delete_room(t_room *room, t_chat *chat) {
    gtk_list_box_unselect_all(chat->listbox_dlgs);
    set_default_settings(room, chat->builder);
    if (room->scrll_wndw_msgs)
        gtk_widget_destroy(GTK_WIDGET(room->scrll_wndw_msgs));
    gtk_widget_destroy(GTK_WIDGET(room->row_chat));
}

void free_room(t_room *room) {
    if (room) {
        mx_free((void **)&(room->chat_name));
        mx_free((void **)&(room));
        room = NULL;
    }
}
