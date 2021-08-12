#include "uchat.h"

void edit_message(GtkButton *btn, t_chat *chat) {
    GtkStack *stack_entry = GTK_STACK(gtk_builder_get_object(chat->builder, "stack_entry"));
    t_room *room = chat->curr_chat;
    gtk_stack_set_visible_child_name(stack_entry, "chat_edit_message");
    GtkEntry *chat_edit_message = GTK_ENTRY(gtk_builder_get_object(chat->builder, "chat_edit_message"));
    GtkListBoxRow *r = gtk_list_box_get_selected_row(room->listbox_msgs);

    t_msg *msg = g_object_get_data(G_OBJECT(r), "msg");
    gtk_entry_set_text(chat_edit_message, msg->msg);
}

void send_edited_message(GtkEntry *entry, t_chat *chat) {
    GtkStack *stack_entry = GTK_STACK(gtk_builder_get_object(chat->builder, "stack_entry"));
    GtkStack *stack_upper_dialog_toolbar = GTK_STACK(gtk_builder_get_object(chat->builder, "stack_upper_dialog_toolbar"));
    GtkEntry *chat_edit_message = GTK_ENTRY(gtk_builder_get_object(chat->builder, "chat_edit_message"));
    t_room *room = chat->curr_chat;
    GtkListBoxRow *r = gtk_list_box_get_selected_row(room->listbox_msgs);
    t_msg *msg = g_object_get_data(G_OBJECT(r), "msg");

    free(msg->msg);
    msg->msg = NULL;
    msg->msg = malloc(sizeof(char) * gtk_entry_get_text_length(chat_edit_message));
    const gchar *text = gtk_entry_get_text(entry);
    strcpy(msg->msg, text);
    char *new_msg = form_msg(msg->sender, msg->time, msg->msg);
    gtk_label_set_text(msg->msg_text, new_msg);
    free(new_msg);
    gtk_stack_set_visible_child_name(stack_entry, "chat_message_entry");
    gtk_stack_set_visible_child_name(stack_upper_dialog_toolbar, "chat_topbar_grid");
    gtk_entry_set_text(chat_edit_message, "");
    gtk_list_box_unselect_all(room->listbox_msgs);
}
