#include "uchat.h"

void connect_profile_settings(t_chat *chat) {
    GtkButton *profile_setting = GTK_BUTTON(gtk_builder_get_object(chat->builder, "profile_setting"));
    GtkButton *btn_log_out = GTK_BUTTON(gtk_builder_get_object(chat->builder, "btn_log_out"));

    g_signal_connect(profile_setting, "clicked", G_CALLBACK(show_profile_settings), chat);
    g_signal_connect(btn_log_out, "clicked", G_CALLBACK(log_out), chat);
}

void show_profile_settings(GtkButton *button, t_chat *chat) {
    GObject *profile_settings = gtk_builder_get_object(chat->builder, "profile_settings");

    gtk_widget_show(GTK_WIDGET(profile_settings));
}

void log_out(GtkButton *btn, t_chat *chat) {
    
}
