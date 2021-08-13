#include "uchat.h"

int N = 0;

static bool row_is_already_in_list(char *user, GtkListBox *box) {
    bool result = false;
    printf("%s\n", user);
    GList *rows = NULL;
    GList *listrunner;

    if (gtk_container_get_children(GTK_CONTAINER(box)) != NULL) {
        rows = gtk_container_get_children(GTK_CONTAINER(box));
        listrunner = g_list_first(rows);    
        while (listrunner) {
            if (strcmp(gtk_widget_get_name(GTK_WIDGET(listrunner->data)), user) == 0) {
                result = true; 
            }
            listrunner = g_list_next(listrunner);
        }
    }
    return result;
}

void connect_search_username_room(t_chat *chat) {
    GtkEntry *chat_search_user = GTK_ENTRY(gtk_builder_get_object(chat->builder, "chat_search_user"));

    g_signal_connect(chat_search_user, "activate", G_CALLBACK(req_search_user), chat);
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
    // }
    char *names[2] = {"1", "5"}; 
    filter_search(names, 2, chat);
}

void filter_search(char **users, int n, t_chat *chat) {
    GtkStack *stack = GTK_STACK(gtk_builder_get_object(chat->builder, "stack"));
    GObject *listbox_found_dlgs = GTK_LIST_BOX(gtk_builder_get_object(chat->builder, "listbox_found_dlgs"));
    GtkLabel *lbl_global_search_nothing_found = GTK_LABEL(gtk_builder_get_object(chat->builder, "lbl_global_search_nothing_found"));
    GtkLabel *lbl_global_search = GTK_LABEL(gtk_builder_get_object(chat->builder, "lbl_global_search"));
    GtkLabel *lbl_local_search = GTK_LABEL(gtk_builder_get_object(chat->builder, "lbl_local_search"));
    GtkLabel *lbl_local_search_nothing_found = GTK_LABEL(gtk_builder_get_object(chat->builder, "lbl_local_search_nothing_found"));
    gtk_widget_show(lbl_local_search);
    gtk_widget_show(lbl_global_search);
    int tmp = 0;
    
    gtk_container_foreach(GTK_CONTAINER(chat->listbox_dlgs), G_CALLBACK(filter_row), (const gchar **)users);
    for (int i = 0; i < n; i++) {
        if (gtk_stack_get_child_by_name(stack, users[i]) == NULL) { 
            if (!row_is_already_in_list(users[i], listbox_found_dlgs)) {
                GtkWidget *r = gtk_list_box_row_new();
                GtkWidget *lbl = gtk_label_new(users[i]);
                gtk_container_add(GTK_CONTAINER(listbox_found_dlgs), r);
                gtk_container_add(GTK_CONTAINER(r), lbl);
                gtk_widget_set_name(r, users[i]);
                gtk_widget_show_all(r);
                tmp++;
            }
        }
    }
    if (gtk_container_get_children(GTK_CONTAINER(listbox_found_dlgs)) == NULL)
        gtk_widget_show(GTK_WIDGET(lbl_global_search_nothing_found));
    else 
        gtk_widget_hide(GTK_WIDGET(lbl_global_search_nothing_found));
    if (tmp == n)
        gtk_widget_show(GTK_WIDGET(lbl_local_search_nothing_found));
    else 
        gtk_widget_hide(GTK_WIDGET(lbl_local_search_nothing_found));
}

void filter_row(GtkWidget *wdg, char **users) {
    char *s = g_object_get_data(G_OBJECT(wdg), "room");
    int n = 0, i = 0;
    
    gtk_widget_hide(wdg);
    for (; users[n] != NULL; n++);
    while (i < n) {
        if (strcmp(s, users[i]) == 0)
            gtk_widget_show(wdg);
        i++;
    }
}
