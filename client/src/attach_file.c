#include "uchat.h"

void attach_file(GtkButton *btn, t_chat *chat) {
    GObject *file_chooser = gtk_builder_get_object(chat->builder, "file_chooser");

    gtk_widget_show_all(GTK_WIDGET(file_chooser));
} 
