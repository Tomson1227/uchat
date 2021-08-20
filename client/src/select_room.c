#include "uchat.h"

void manage_room_visibility(t_chat *chat) {
    GtkBox *bottom_bar_chat = GTK_BOX(gtk_builder_get_object(chat->builder, "bottom_bar_chat"));
    GtkStack *stack_entry = GTK_STACK(gtk_builder_get_object(chat->builder, "stack_entry"));
    GtkStack *stack_upper_dialog_toolbar = GTK_STACK(gtk_builder_get_object(chat->builder, "stack_upper_dialog_toolbar"));

    gtk_widget_show_all(GTK_WIDGET(bottom_bar_chat));
    gtk_stack_set_visible_child_name(stack_entry, "chat_message_entry");
    gtk_stack_set_visible_child_name(stack_upper_dialog_toolbar, "chat_topbar_grid");
    gtk_widget_show_all(GTK_WIDGET(stack_upper_dialog_toolbar));
}

void select_room(GtkListBox *box, GtkListBoxRow *row, t_chat *chat) {
    GtkLabel *lbl_room_name = GTK_LABEL(gtk_builder_get_object(chat->builder, "lbl_room_name"));
    GtkButton *room_info = GTK_BUTTON(gtk_builder_get_object(chat->builder, "room_info"));

    manage_room_visibility(chat);
    if (g_object_get_data(G_OBJECT(row), "room") != NULL) {
        char *tmp = g_object_get_data(G_OBJECT(row), "room_name");
        GtkStack *stack = GTK_STACK(gtk_builder_get_object(chat->builder, "stack"));
        GtkWidget *scroll = gtk_stack_get_child_by_name(stack, tmp);
        gtk_stack_set_visible_child_name(stack, tmp);
        t_room *room = g_object_get_data(G_OBJECT(scroll), "dialog");
        gtk_label_set_text(lbl_room_name, room->chat_name);
        gtk_button_set_label(room_info, room->chat_name);
        chat->curr_chat = room;
        char *temp = send_req_upload_messages(room->room_id);
        enQueue(chat->config->queue_send, temp);
    }
}
