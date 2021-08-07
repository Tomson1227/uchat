#include "uchat.h"

// void window_room_info_hide(GObject *wndw, t_chat *chat) {
//     gtk_widget_hide(GTK_WIDGET(wndw));
// }

void show_room_settings(GtkButton *button, t_chat *chat) {
    GObject *profile_settings = gtk_builder_get_object(chat->builder, "profile_settings");

    gtk_widget_show(GTK_WIDGET(profile_settings));
}
