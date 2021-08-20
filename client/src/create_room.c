#include "uchat.h"

static t_room *fill_room(int id, char *rs, t_chat *chat); 
static void add_dialog_row(t_room *room, t_chat *chat);
static void add_messages_box(t_room *room, t_chat *chat);
static void init_room(t_room *room, t_chat *chat);

void display_upper_panel(GtkListBox *box, GtkListBoxRow *r, GtkStack *stack_upper_dialog_toolbar) {
    if (r) 
        gtk_stack_set_visible_child_name (stack_upper_dialog_toolbar, "chat_topbar_wth_btns");
}

static void add_dialog_row(t_room *room, t_chat *chat) {
    chat->listbox_dlgs = GTK_LIST_BOX(gtk_builder_get_object(chat->builder, "listbox_dlgs"));
    GtkWidget *row = gtk_list_box_row_new();
    char *id = my_itoa(room->room_id);
    GtkWidget *lbl = gtk_label_new(room->chat_name);
    g_signal_connect(chat->listbox_dlgs, "row-selected", G_CALLBACK(select_room), chat);
     
    gtk_container_add(GTK_CONTAINER(chat->listbox_dlgs), row);
    gtk_widget_show(row);
    room->row_chat = GTK_LIST_BOX_ROW(row);
    gtk_container_add(GTK_CONTAINER(row), lbl);
    gtk_widget_show(lbl);
    g_object_set_data(G_OBJECT(row), "room", id);
    g_object_set_data(G_OBJECT(row), "room_name", room->chat_name);
    manage_room_visibility(chat);
    gtk_list_box_select_row(chat->listbox_dlgs, room->row_chat);
}

static void add_messages_box(t_room *room, t_chat *chat) {
    manage_room_visibility(chat);
    GObject *stack = gtk_builder_get_object(chat->builder, "stack");
    GtkWidget *box = gtk_list_box_new();
    GtkWidget *scroll = gtk_scrolled_window_new(NULL, NULL);
    GtkWidget *view = gtk_viewport_new(NULL, NULL);
    GObject *stack_upper_dialog_toolbar = gtk_builder_get_object(chat->builder, "stack_upper_dialog_toolbar");

    gtk_container_add(GTK_CONTAINER(scroll), view);
    gtk_container_add(GTK_CONTAINER(view), box);
    room->listbox_msgs = GTK_LIST_BOX(box);
    g_signal_connect(box, "row-selected", G_CALLBACK(display_upper_panel), stack_upper_dialog_toolbar);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scroll), GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);
    gtk_stack_add_named(GTK_STACK(stack), scroll, room->chat_name);
    gtk_stack_set_visible_child(GTK_STACK(stack), scroll);
    room->stack = GTK_STACK(stack);
    room->scrll_wndw_msgs = GTK_SCROLLED_WINDOW(scroll);
    if (scroll)
        gtk_widget_show_all(scroll);
    g_object_set_data(G_OBJECT(scroll), "dialog", room);
}

static t_room *fill_room(int id, char *name, t_chat *chat) {
    t_room *room = (t_room *)malloc(sizeof(t_room));
    init_room(room, chat);

    room->room_id = id;
    room->chat_name = mx_strdup(name);
    return room;
} 

static void init_room(t_room *room, t_chat *chat) {
    room->stack = NULL;
    room->scrll_wndw_msgs = NULL;
    room->vwprt = NULL;
    room->listbox_msgs = NULL;
    room->row_chat = NULL;
    room->is_updated = FALSE;
    room->members = NULL;
}

void req_create_dialog(GtkListBox *box, GtkListBoxRow *row, t_chat *chat) {
    char *customer = NULL;

    if (gtk_widget_get_name(GTK_WIDGET(row)) != NULL) {
        customer = malloc(sizeof(char) * strlen(gtk_widget_get_name(GTK_WIDGET(row))));
        sprintf(customer, "%s", gtk_widget_get_name(GTK_WIDGET(row)));
        char *temp = send_rq_create_room_client(chat->username, customer);
        enQueue(chat->config->queue_send, temp);
    }
}

void create_dialog(int id, char *name, t_chat *chat) {
    GtkListBox *box = GTK_LIST_BOX(gtk_builder_get_object(chat->builder, "listbox_found_dlgs"));
    manage_visibility(box, chat);
    t_room *room = fill_room(id, name, chat);
    add_messages_box(room, chat);
    add_dialog_row(room, chat);
    chat->curr_chat = room;
}

static void clear_listbox_with_found_messages(GtkListBox *box) {
    GList *rows = NULL;
    GList *listrunner;

    if (gtk_container_get_children(GTK_CONTAINER(box)) != NULL) {
        rows = gtk_container_get_children(GTK_CONTAINER(box));
        listrunner = g_list_first(rows);    
        while (listrunner) {
            gtk_container_remove(GTK_CONTAINER(box), listrunner->data);
            listrunner = g_list_next(listrunner);
        }
    }
}

void manage_visibility(GtkListBox *box, t_chat *chat) {
    GtkEntry *chat_search_user = GTK_ENTRY(gtk_builder_get_object(chat->builder, "chat_search_user"));
    GtkLabel *lbl_local_search = GTK_LABEL(gtk_builder_get_object(chat->builder,"lbl_local_search"));
    GtkLabel *lbl_local_search_nothing_found = GTK_LABEL(gtk_builder_get_object(chat->builder, "lbl_local_search_nothing_found"));
    GtkLabel *lbl_global_search = GTK_LABEL(gtk_builder_get_object(chat->builder, "lbl_global_search"));
    GtkLabel *lbl_global_search_nothing_found = GTK_LABEL(gtk_builder_get_object(chat->builder, "lbl_global_search_nothing_found"));
    
    gtk_widget_show_all(GTK_WIDGET(chat->listbox_dlgs));
    gtk_list_box_unselect_all(box);
    if (gtk_container_get_children(GTK_CONTAINER(box)) != NULL)
        clear_listbox_with_found_messages(box);
    gtk_widget_hide(GTK_WIDGET(box));
    gtk_widget_hide(GTK_WIDGET(lbl_local_search));
    gtk_widget_hide(GTK_WIDGET(lbl_local_search_nothing_found));
    gtk_widget_hide(GTK_WIDGET(lbl_global_search_nothing_found));
    gtk_widget_hide(GTK_WIDGET(lbl_global_search));
    gtk_entry_set_text(chat_search_user, ""); 
}
