#include "uchat.h"

void delete_room(GtkButton *btn, t_chat *chat) {
    t_room *room = chat->curr_chat;
    //send request 
    
    //room->room_id
    //free messages
    gtk_widget_destroy(room->row_chat);
    gtk_widget_destroy(room->scrll_wndw_msgs);
    
    GtkEntry *chat_message_entry = GTK_ENTRY(gtk_builder_get_object(chat->builder, "chat_message_entry"));
    GtkButton *btn_send_msg = GTK_BUTTON(gtk_builder_get_object(chat->builder, "btn_send_msg"));
    GtkButton *btn_send_sticker = GTK_BUTTON(gtk_builder_get_object(chat->builder, "btn_send_sticker"));
    GtkButton *room_info = GTK_BUTTON(gtk_builder_get_object(chat->builder, "room_info"));
    GtkButton *btn_attach_file = GTK_BUTTON(gtk_builder_get_object(chat->builder, "btn_attach_file"));
    
    gtk_widget_hide(GTK_WIDGET(room_info));
    gtk_widget_hide(GTK_WIDGET(btn_send_msg));
    gtk_widget_hide(GTK_WIDGET(btn_send_sticker));
    gtk_widget_hide(GTK_WIDGET(chat_message_entry));
    gtk_widget_hide(GTK_WIDGET(btn_attach_file));
}
