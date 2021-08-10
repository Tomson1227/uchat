#include "uchat.h"

int N = 0;

void connect_search_username_room(t_chat *chat) {
    GtkEntry *chat_search_user = GTK_ENTRY(gtk_builder_get_object(chat->builder, "chat_search_user"));

    g_signal_connect(chat_search_user, "changed", G_CALLBACK(req_search_user), chat);
    //send request
}

void req_search_user(GtkEntry *entry, t_chat *chat) {
    const gchar *text = gtk_entry_get_text(entry);

    // if (strlen(text) > 6) {
    //     char *tmp = send_rq_search_username(text);
    //     enQueue(chat->config->queue_send, tmp);
    //     while(QueueisEmpty(chat->config->queue_recv)) {
    //     }
    //     char *dq = deQueue(chat->config->queue_recv);
    //     process_rs_client(dq, chat);
    // }
    char *names[2] = {"1", "2"}; 
    filter_search(names, 2, chat);
}

void filter_search(char **users, int n, t_chat *chat) {
    

    GtkListBox *listbox_dlgs = GTK_LIST_BOX(gtk_builder_get_object(chat->builder, "listbox_dlgs"));

    for (int i = 0; i < n; i++) {
        gtk_container_foreach(listbox_dlgs, G_CALLBACK(filter_row), users[i]);
    }
}

void filter_row(GtkWidget *wdg, char *user) {
    char *s = g_object_get_data(wdg, "room");
    printf("user: %s\n", user);
    printf("got data: %s\n", s);
    if (strcmp(s, user) != 0) {
        gtk_widget_hide(wdg);
    }
}
