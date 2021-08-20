#include "uchat.h"

static void init_fields(t_chat *chat) {
    chat->stack_dialog_box = NULL;
    chat->stack_upper_dialog_toolbar = NULL;
    chat->scrll_wndw_dlgs = NULL;
    chat->label_msg = NULL;
    chat->row = NULL;
    chat->lbl_slct_chat = NULL;
    chat->row_slctd = NULL;
    chat->listbox_dlgs = NULL;
    chat->curr_chat = NULL;
    chat->username = NULL;
    chat->username_id = -1;
    chat->description = NULL;
}

GtkBuilder *init_window(t_chat *chat) {
    GtkBuilder *builder;

    gtk_init(NULL, NULL);
    builder = gtk_builder_new();
    gtk_builder_add_from_file(builder, "client/glades/chat_window.glade", NULL);
    gtk_builder_connect_signals(builder, builder);
    return builder;
}

t_chat *init_chat(t_chat *chat, t_config *config) {
    chat = (t_chat *)malloc(sizeof(t_chat));
    chat->config = config;
    chat->builder = init_window(chat);
    init_fields(chat);
    return chat;
}

static void reset_sign_in_up_windows(t_chat *chat) {
    GtkEntry *username = GTK_ENTRY(gtk_builder_get_object(chat->builder, "username"));
    GtkEntry *password = GTK_ENTRY(gtk_builder_get_object(chat->builder, "password"));
    GtkEntry *username_sign_up = GTK_ENTRY(gtk_builder_get_object(chat->builder, "username_sign_up"));
    GtkEntry *password_sign_up = GTK_ENTRY(gtk_builder_get_object(chat->builder, "password_sign_up"));
    GtkEntry *password_repeated = GTK_ENTRY(gtk_builder_get_object(chat->builder, "repeat_password_sign_up"));

    gtk_entry_set_text(username, "");
    gtk_entry_set_text(password, "");
    gtk_entry_set_text(username_sign_up, "");
    gtk_entry_set_text(password_sign_up, "");
    gtk_entry_set_text(password_repeated, "");
}

void init_chat_window(t_chat *chat, int type) {
    reset_sign_in_up_windows(chat);
    GObject *wnd_main = gtk_builder_get_object(chat->builder, "window_main");
    gtk_widget_set_visible(GTK_WIDGET(wnd_main), FALSE);  
    GObject *chat_wndw = gtk_builder_get_object(chat->builder, "chat_main");
    GtkStack *stack_entry = GTK_STACK(gtk_builder_get_object(chat->builder, "stack_entry"));
    GtkButton *btn_send_msg = GTK_BUTTON(gtk_builder_get_object(chat->builder, "btn_send_msg"));
    GtkLabel *lbl = GTK_LABEL(gtk_builder_get_object(chat->builder, "lbl_username"));
    GtkStack *stack_upper_dialog_toolbar = GTK_STACK(gtk_builder_get_object(chat->builder, "stack_upper_dialog_toolbar"));
    GtkLabel *lbl_local_search = GTK_LABEL(gtk_builder_get_object(chat->builder,"lbl_local_search"));
    GtkLabel *lbl_local_search_nothing_found = GTK_LABEL(gtk_builder_get_object(chat->builder, "lbl_local_search_nothing_found"));
    GtkLabel *lbl_global_search = GTK_LABEL(gtk_builder_get_object(chat->builder, "lbl_global_search"));
    GtkLabel *lbl_global_search_nothing_found = GTK_LABEL(gtk_builder_get_object(chat->builder, "lbl_global_search_nothing_found"));

    gtk_widget_show(GTK_WIDGET(chat_wndw));
    gtk_label_set_text(lbl, chat->username);
    gtk_widget_hide(GTK_WIDGET(stack_upper_dialog_toolbar));
    gtk_widget_hide(GTK_WIDGET(btn_send_msg));
    gtk_widget_hide(GTK_WIDGET(stack_entry));
    gtk_widget_hide(GTK_WIDGET(lbl_local_search));
    gtk_widget_hide(GTK_WIDGET(lbl_local_search_nothing_found));
    gtk_widget_hide(GTK_WIDGET(lbl_global_search_nothing_found));
    gtk_widget_hide(GTK_WIDGET(lbl_global_search));
    
    if (type == 0) {
        char *temp = send_req_old_dialogs(chat->username);
        enQueue(chat->config->queue_send, temp);
    }
}
