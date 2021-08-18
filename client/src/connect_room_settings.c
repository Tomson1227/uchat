#include "uchat.h"

void connect_room_settings(t_chat *chat) {
    GtkButton *room_info = GTK_BUTTON(gtk_builder_get_object(chat->builder, "room_info"));
    GtkButton *btn_clear_history = GTK_BUTTON(gtk_builder_get_object(chat->builder, "btn_clear_history"));
    GtkButton *btn_delete_room = GTK_BUTTON(gtk_builder_get_object(chat->builder, "btn_delete_room"));
    GtkButton *btn_block_user = GTK_BUTTON(gtk_builder_get_object(chat->builder, "btn_block_user"));
    GtkButton *btn_confirm_clear_history = GTK_BUTTON(gtk_builder_get_object(chat->builder, "btn_confirm_clear_history"));
    GtkButton *btn_cancel_clear_history = GTK_BUTTON(gtk_builder_get_object(chat->builder, "btn_cancel_clear_history"));
    GtkButton *btn_confirm_delete_room = GTK_BUTTON(gtk_builder_get_object(chat->builder, "btn_confirm_delete_room"));
    GtkButton *btn_cancel_delete_room = GTK_BUTTON(gtk_builder_get_object(chat->builder, "btn_cancel_delete_room"));
    GtkButton *btn_confirm_block_user = GTK_BUTTON(gtk_builder_get_object(chat->builder, "btn_confirm_block_user"));
    GtkButton *btn_cancel_block_user = GTK_BUTTON(gtk_builder_get_object(chat->builder, "btn_cancel_block_user"));
    GtkButton *btn_close_info = GTK_BUTTON(gtk_builder_get_object(chat->builder, "btn_close_info"));

    g_signal_connect(btn_clear_history, "clicked", G_CALLBACK(show_clear_history_window), chat);
    g_signal_connect(room_info, "clicked", G_CALLBACK(show_room_info), chat);
    g_signal_connect(btn_delete_room, "clicked", G_CALLBACK(show_delete_room_window), chat);
    g_signal_connect(btn_block_user, "clicked", G_CALLBACK(show_block_user_window), chat);
    g_signal_connect(btn_confirm_clear_history, "clicked", G_CALLBACK(clear_history), chat);
    g_signal_connect(btn_cancel_clear_history, "clicked", G_CALLBACK(cancel_clear_history), chat);
    g_signal_connect(btn_confirm_delete_room, "clicked", G_CALLBACK(req_delete_room), chat);
    g_signal_connect(btn_cancel_delete_room, "clicked", G_CALLBACK(cancel_delete_room), chat);
    g_signal_connect(btn_confirm_block_user, "clicked", G_CALLBACK(block_user), chat);
    g_signal_connect(btn_cancel_block_user, "clicked", G_CALLBACK(cancel_block_user), chat);
    g_signal_connect(btn_close_info, "clicked", G_CALLBACK(close_info), chat);
}

void show_room_info(GtkButton *btn, t_chat *chat) {
    GObject *wndw_room_info = gtk_builder_get_object(chat->builder, "wndw_room_info");
    GtkBox *room_info_lstbx = GTK_BOX(gtk_builder_get_object(chat->builder, "room_info_lstbx"));
   
    gtk_widget_show(GTK_WIDGET(wndw_room_info));
    gtk_widget_show(GTK_WIDGET(room_info_lstbx));
}

void show_clear_history_window(GtkButton *btn, t_chat *chat) {
    GObject *clear_history = gtk_builder_get_object(chat->builder, "clear_history");

    gtk_widget_show(GTK_WIDGET(clear_history));
}

void show_delete_room_window(GtkButton *btn, t_chat *chat) {
    GObject *delete_room = gtk_builder_get_object(chat->builder, "delete_room");

    gtk_widget_show(GTK_WIDGET(delete_room));
}

void show_block_user_window(GtkButton *btn, t_chat *chat) {
    GObject *block_user = gtk_builder_get_object(chat->builder, "block_user");

    gtk_widget_show(GTK_WIDGET(block_user));
}

void clear_history(GtkButton *btn, t_chat *chat) {
    GObject *clear_history = gtk_builder_get_object(chat->builder, "clear_history");
    GObject *wndw_room_info = gtk_builder_get_object(chat->builder, "wndw_room_info");
    
    gtk_widget_hide(GTK_WIDGET(clear_history));
    gtk_widget_hide(GTK_WIDGET(wndw_room_info));
    gtk_container_foreach(GTK_CONTAINER(chat->curr_chat->listbox_msgs), (GtkCallback)delete_msg, chat->curr_chat);
}

void cancel_clear_history(GtkButton *btn, t_chat *chat) {
    GObject *clear_history = gtk_builder_get_object(chat->builder, "clear_history");

    gtk_widget_hide(GTK_WIDGET(clear_history));
}

void cancel_delete_room(GtkButton *btn, t_chat *chat) {
    GObject *delete_room_wnd = gtk_builder_get_object(chat->builder, "delete_room");

    gtk_widget_hide(GTK_WIDGET(delete_room_wnd));
}

void block_user(GtkButton *btn, t_chat *chat) {

}

void cancel_block_user(GtkButton *btn, t_chat *chat) {
    GObject *block_user = gtk_builder_get_object(chat->builder, "block_user");

    gtk_widget_hide(GTK_WIDGET(block_user));
}

void close_info(GtkButton *btn, t_chat *chat) {
    GObject *wndw_room_info = gtk_builder_get_object(chat->builder, "wndw_room_info");

    gtk_widget_hide(GTK_WIDGET(wndw_room_info));
}

 void on_close_edit_profile_clicked(GtkButton *btn, GObject *wnd) {
     gtk_widget_hide(GTK_WIDGET(wnd));
 }

void on_btn_close_menu_clicked(GtkButton *btn, GObject *wnd) {
    gtk_widget_hide(GTK_WIDGET(wnd));
}

void on_close_chat_clicked(GtkButton *btn, GObject *wnd) {
    gtk_widget_hide(GTK_WIDGET(wnd));
}
