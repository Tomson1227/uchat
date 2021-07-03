#pragma once

#include <gtk/gtk.h>

typedef struct 
{
    GtkEntry *username;
    GtkEntry *password;
    GtkWidget *g_lbl_hello;
    GtkWidget *g_lbl_count;
    GtkWidget *g_lbl_error;
    GtkGrid *log_in;
    GtkGrid *sign_up;
    GtkStack *stack;
    GtkEntry *username_sign_up;
    GtkEntry *password_sign_up;
    GtkEntry *repeat_password_sign_up;
    GtkGrid *grid;
    GtkButton *btn_back;
} app_widgets;
