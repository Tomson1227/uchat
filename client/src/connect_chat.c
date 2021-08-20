#include "uchat.h"

void on_chat_main_destroy() {
    gtk_main_quit();
}

void on_btn_cancel_clicked(GtkButton *btn_cancel, GtkStack *stack_upper_dialog_toolbar) {
    gtk_stack_set_visible_child_full (stack_upper_dialog_toolbar, "chat_topbar_grid", 0);
}

void on_chat_settings_btn_clicked(GtkButton *chat_settings_settings_btn, GtkStack *stack_dialog_box) {
    if (strcmp(gtk_stack_get_visible_child_name(stack_dialog_box), "scrll_wndw_dlgs1") == 0) {
        gtk_stack_set_visible_child_name (stack_dialog_box, "scrll_wndw_dlgs");    
        gtk_stack_set_transition_type(stack_dialog_box, GTK_STACK_TRANSITION_TYPE_SLIDE_LEFT);
    } 
    else if (strcmp(gtk_stack_get_visible_child_name(stack_dialog_box), "scrll_wndw_dlgs") == 0) {
        gtk_stack_set_visible_child_name (stack_dialog_box, "scrll_wndw_dlgs1");
        gtk_stack_set_transition_type(stack_dialog_box, GTK_STACK_TRANSITION_TYPE_SLIDE_RIGHT);
    }
}


void connect_chat(t_chat *chat) {
    GtkButton *btn_send_msg = GTK_BUTTON(gtk_builder_get_object(chat->builder, "btn_send_msg"));
    GtkEntry *chat_message_entry = GTK_ENTRY(gtk_builder_get_object(chat->builder, "chat_message_entry"));
    GtkButton *btn_delete_msg = GTK_BUTTON(gtk_builder_get_object(chat->builder, "btn_delete_msg"));
    GtkButton *btn_edit_msg = GTK_BUTTON(gtk_builder_get_object(chat->builder, "btn_edit_msg"));
    GtkEntry *chat_edit_message = GTK_ENTRY(gtk_builder_get_object(chat->builder, "chat_edit_message"));
    GtkListBox *listbox_found_dlgs = GTK_LIST_BOX(gtk_builder_get_object(chat->builder, "listbox_found_dlgs"));
    
    g_signal_connect(btn_send_msg, "clicked", G_CALLBACK(on_btn_send_message_clicked), chat);
    g_signal_connect(chat_message_entry, "activate", G_CALLBACK(req_send_message), chat);
    g_signal_connect(btn_delete_msg, "clicked", G_CALLBACK(req_delete_msg), chat);
    g_signal_connect(chat_edit_message, "activate", G_CALLBACK(send_edited_message), chat);
    g_signal_connect(btn_edit_msg, "clicked", G_CALLBACK(edit_message), chat);
    g_signal_connect(listbox_found_dlgs, "row-selected", G_CALLBACK(req_create_dialog), chat);
}
