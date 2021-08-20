#include "uchat.h"

void connect_profile_settings(t_chat *chat) {
    GtkButton *btn_log_out = GTK_BUTTON(gtk_builder_get_object(chat->builder, "btn_log_out")); 
    GtkButton *btn_confirm_log_out = GTK_BUTTON(gtk_builder_get_object(chat->builder, "btn_confirm_log_out"));

    g_signal_connect(btn_confirm_log_out, "clicked", G_CALLBACK(log_out), chat);
    g_signal_connect(btn_log_out, "clicked", G_CALLBACK(show_log_out_window), chat);
}

void show_log_out_window(GtkButton *btn, t_chat *chat) {
    GObject *log_out = gtk_builder_get_object(chat->builder, "log_out");

    gtk_widget_show(GTK_WIDGET(log_out));
}

void log_out(GtkButton *btn, t_chat *chat) {
    GObject *log_out = gtk_builder_get_object(chat->builder, "log_out");
    GObject *chat_main = gtk_builder_get_object(chat->builder, "chat_main");
    GObject *window_main = gtk_builder_get_object(chat->builder, "window_main");
    
    gtk_widget_hide(GTK_WIDGET(log_out));
    gtk_widget_hide(GTK_WIDGET(chat_main));
    gtk_widget_show(GTK_WIDGET(window_main));
}

void on_btn_cancel_log_out_clicked(GtkButton *btn, GObject *log_out) {
    gtk_widget_hide(GTK_WIDGET(log_out));
}
