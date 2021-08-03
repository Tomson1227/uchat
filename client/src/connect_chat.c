#include "uchat.h"

void on_chat_main_destroy() {
    gtk_main_quit();
}

void on_btn_cancel_clicked(GtkButton *btn_cancel, GtkStack *stack_upper_dialog_toolbar) {
    gtk_stack_set_visible_child_full (stack_upper_dialog_toolbar, "chat_topbar_grid", 0);
}

void on_chat_settings_btn_clicked(GtkButton *chat_settings_settings_btn, GtkStack *stack_dialog_box) {
    if (strcmp(gtk_stack_get_visible_child_name(stack_dialog_box), "scrll_wndw_dlgs1") == 0) 
        gtk_stack_set_visible_child_name (stack_dialog_box, "scrll_wndw_dlgs");    
    else if (strcmp(gtk_stack_get_visible_child_name(stack_dialog_box), "scrll_wndw_dlgs") == 0)  
        gtk_stack_set_visible_child_name (stack_dialog_box, "scrll_wndw_dlgs1");
}

void on_chat_settings_terminal_btn_clicked(GtkButton *chat_settings_terminal_btn, t_chat *wdgts) {
}

void connect_chat(t_chat *chat) {
    GtkButton *btn_send_msg = GTK_BUTTON(gtk_builder_get_object(chat->builder, "btn_send_msg"));
    GtkEntry *chat_message_entry = GTK_ENTRY(gtk_builder_get_object(chat->builder, "chat_message_entry"));
    GtkButton *btn_create_room = GTK_BUTTON(gtk_builder_get_object(chat->builder, "btn_create_room"));
    GtkButton *btn_delete_msg = GTK_BUTTON(gtk_builder_get_object(chat->builder, "btn_delete_msg"));
    
    g_signal_connect(btn_send_msg, "clicked", G_CALLBACK(send_message), chat);
    g_signal_connect(chat_message_entry, "activate", G_CALLBACK(send_message), chat);
    g_signal_connect(btn_create_room, "clicked", G_CALLBACK(create_room), chat);
    // g_signal_connect(btn_delete_msg, "clicked", G_CALLBACK(delete_msg), chat);
}
