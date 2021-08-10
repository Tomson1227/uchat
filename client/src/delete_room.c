#include "uchat.h"

void delete_room(GtkButton *btn, t_chat *chat) {
    printf("entered the function\n");
    t_room *room = chat->curr_chat;
    if (!room) 
        printf("no room\n");
    //send request 
    
    //room->room_id
    //free messages
    gtk_widget_hide(GTK_WIDGET(room->row_chat));
    gtk_widget_hide(GTK_WIDGET(room->scrll_wndw_msgs));
    // chat->curr_chat = NULL;
    
    GtkEntry *chat_message_entry = GTK_ENTRY(gtk_builder_get_object(chat->builder, "chat_message_entry"));
    GtkButton *btn_send_msg = GTK_BUTTON(gtk_builder_get_object(chat->builder, "btn_send_msg"));
    GtkButton *btn_send_sticker = GTK_BUTTON(gtk_builder_get_object(chat->builder, "btn_send_sticker"));
    GtkButton *room_info = GTK_BUTTON(gtk_builder_get_object(chat->builder, "room_info"));
    GtkButton *btn_attach_file = GTK_BUTTON(gtk_builder_get_object(chat->builder, "btn_attach_file"));
    GObject *delete_room = gtk_builder_get_object(chat->builder, "delete_room");
    GObject *wndw_room_info = gtk_builder_get_object(chat->builder, "wndw_room_info");
    

    gtk_widget_hide(GTK_WIDGET(wndw_room_info));
    gtk_widget_hide(GTK_WIDGET(delete_room));
    gtk_widget_hide(GTK_WIDGET(room_info));
    gtk_widget_hide(GTK_WIDGET(btn_send_msg));
    gtk_widget_hide(GTK_WIDGET(btn_send_sticker));
    gtk_widget_hide(GTK_WIDGET(chat_message_entry));
    gtk_widget_hide(GTK_WIDGET(btn_attach_file));
}
