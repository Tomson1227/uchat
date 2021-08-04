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

void init_chat_window(t_chat *chat) {
    GObject *chat_wndw = gtk_builder_get_object(chat->builder, "chat_main");
    GtkEntry *chat_message_entry = GTK_ENTRY(gtk_builder_get_object(chat->builder, "chat_message_entry"));
    GtkButton *btn_send_msg = GTK_BUTTON(gtk_builder_get_object(chat->builder, "btn_send_msg"));
    GtkButton *btn_send_sticker = GTK_BUTTON(gtk_builder_get_object(chat->builder, "btn_send_sticker"));
    GtkLabel *lbl = GTK_LABEL(gtk_builder_get_object(chat->builder, "lbl_username"));
     
    gtk_widget_show(GTK_WIDGET(chat_wndw));
    gtk_label_set_text(lbl, chat->username);
    gtk_widget_hide(GTK_WIDGET(btn_send_msg));
    gtk_widget_hide(GTK_WIDGET(btn_send_sticker));
    gtk_widget_hide(GTK_WIDGET(chat_message_entry));
}
