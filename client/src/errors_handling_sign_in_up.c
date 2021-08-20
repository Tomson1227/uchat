#include "uchat.h"

void display_error_wrong_username_login(t_chat *chat) {
    GtkLabel *lbl = GTK_LABEL(gtk_builder_get_object(chat->builder, "lbl_error_log_in"));
    gtk_label_set_text(lbl, "wrond username");
}

void display_error_wrong_password_login(t_chat *chat) {
    GtkLabel *lbl = GTK_LABEL(gtk_builder_get_object(chat->builder, "lbl_error_log_in"));
    gtk_label_set_text(lbl, "wrond password");
}

void display_error_user_exists(t_chat *chat) {
    GtkLabel *lbl = GTK_LABEL(gtk_builder_get_object(chat->builder, "lbl_error_display"));
    gtk_label_set_text(lbl, "username already exists");
}
