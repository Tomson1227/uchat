#include "uchat.h"

void show_room_info(GtkButton *btn, t_chat *chat) {
    GObject *wndw_room_info = gtk_builder_get_object(chat->builder, "wndw_room_info");
    GtkBox *room_info_lstbx = GTK_BOX(gtk_builder_get_object(chat->builder, "room_info_lstbx"));
    GtkLabel *room_name = GTK_LABEL(gtk_builder_get_object(chat->builder, "room_name"));
    GtkLabel *room_descr = GTK_LABEL(gtk_builder_get_object(chat->builder, "room_descr"));
    GtkButton *clear_history = GTK_BUTTON(gtk_builder_get_object(chat->builder, "clear_history"));
    GtkButton *delete_room = GTK_BUTTON(gtk_builder_get_object(chat->builder, "delete_room"));
    GtkButton *block_user = GTK_BUTTON(gtk_builder_get_object(chat->builder, "block_user"));

    gtk_widget_show(GTK_WIDGET(room_name));
    gtk_widget_show(GTK_WIDGET(block_user));
    gtk_widget_show(GTK_WIDGET(delete_room));
    gtk_widget_show(GTK_WIDGET(clear_history));
    gtk_widget_show(GTK_WIDGET(wndw_room_info));
    gtk_widget_show_all(GTK_WIDGET(room_info_lstbx));
}

void window_room_info_hide(GObject *wndw, t_chat *chat) {
    gtk_widget_hide(wndw);
}
