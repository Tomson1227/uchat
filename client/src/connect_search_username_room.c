#include "uchat.h"

int N = 0;

void connect_search_username_room(t_chat *chat) {
    GtkEntry *chat_search_user = GTK_ENTRY(gtk_builder_get_object(chat->builder, "chat_search_user"));

    g_signal_connect(chat_search_user, "changed", G_CALLBACK(req_search_user), chat);
    g_signal_connect(chat_search_user, "delete-text", G_CALLBACK(restore_listbox), chat->listbox_dlgs);
    //send request
}

void restore_listbox(GtkEntry *entry, GtkListBox *box) {
    gtk_widget_show_all(GTK_WIDGET(box));
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
    char *names[2] = {"1", "5"}; 
    filter_search(names, 2, chat);
}

void filter_search(char **users, int n, t_chat *chat) {
    GtkStack *stack = GTK_STACK(gtk_builder_get_object(chat->builder, "stack"));

    gtk_container_foreach(GTK_CONTAINER(chat->listbox_dlgs), G_CALLBACK(filter_row), (const gchar **)users);
    for (int i = 0; i < n; i++) {
        if (gtk_stack_get_child_by_name(stack, users[i]) == NULL) {
            GtkWidget *r = gtk_list_box_row_new();
            GtkWidget *lbl = gtk_label_new(users[i]);
            gtk_container_add(GTK_CONTAINER(chat->listbox_dlgs), r);
            gtk_container_add(GTK_CONTAINER(r), lbl);
            gtk_widget_show_all(r);
        }
    }
}

void filter_row(GtkWidget *wdg, char **users) {
    char *s = g_object_get_data(G_OBJECT(wdg), "room");
    printf("got data: %s\n", s);
    int n = 0, i = 0;
    
    gtk_widget_hide(wdg);
    for (; users[n] != NULL; n++);
    while (i < n) {
        if (strcmp(s, users[i]) == 0)
            gtk_widget_show(wdg);
        i++;
    }
}
