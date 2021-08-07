#include "uchat.h"

void connect_search_username_room(t_chat *chat) {
    GtkEntry *chat_search_entry = GTK_ENTRY(gtk_builder_get_object(chat->builder, "chat_search_entry"));

    const gchar *text = gtk_entry_get_text(chat_search_entry);
    //send request
}
