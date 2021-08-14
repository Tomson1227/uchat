#include "uchat.h"

int i = 0;

static t_room *fill_room(int id, char *rs, t_chat *chat);
static void manage_room_visibility(t_chat *chat); 
static void add_dialog_row(t_room *room, t_chat *chat, const gchar *name);
static void add_messages_box(t_room *room, t_chat *chat, const gchar *name);
static void init_room(t_room *room, t_chat *chat);

void display_upper_panel(GtkListBox *box, GtkListBoxRow *r, GtkStack *stack_upper_dialog_toolbar) {
    // int id = g_object_get_data(r, "msg_id");
    // printf("selected msg with id: %d\n", id);
    if (r) 
        gtk_stack_set_visible_child_name (stack_upper_dialog_toolbar, "chat_topbar_wth_btns");
}

void select_room(GtkListBox *box, GtkListBoxRow *row, t_chat *chat) {
    manage_room_visibility(chat);
    if (g_object_get_data(G_OBJECT(row), "room") != NULL) {
        char *tmp = g_object_get_data(G_OBJECT(row), "room");
        GtkStack *stack = GTK_STACK(gtk_builder_get_object(chat->builder, "stack"));
        GtkWidget *scroll = gtk_stack_get_child_by_name(stack, tmp);
        gtk_stack_set_visible_child_name(stack, tmp);
        t_room *room = g_object_get_data(G_OBJECT(scroll), "dialog");
        chat->curr_chat = room;
        // if (!room->is_updated) {
        //     char *temp = send_req_upload_messages(READ_ALL, room->room_id);
        //     enQueue(chat->config->queue_send, temp);
        //     //free(temp);
        // }
        // else {
        //     char *temp = send_req_upload_messages(READ_RECENT, room->room_id);
        //     enQueue(chat->config->queue_send, temp);
        //     //free(temp);
        // }
    }
}

static void manage_room_visibility(t_chat *chat) {
    GtkStack *stack_entry = GTK_STACK(gtk_builder_get_object(chat->builder, "stack_entry"));
    GtkButton *btn_send_msg = GTK_BUTTON(gtk_builder_get_object(chat->builder, "btn_send_msg"));
    GtkButton *btn_send_sticker = GTK_BUTTON(gtk_builder_get_object(chat->builder, "btn_send_sticker"));
    GtkButton *room_info = GTK_BUTTON(gtk_builder_get_object(chat->builder, "room_info"));
    GtkButton *btn_attach_file = GTK_BUTTON(gtk_builder_get_object(chat->builder, "btn_attach_file"));
    GObject *create_group = gtk_builder_get_object(chat->builder, "create_group");

    gtk_widget_hide(GTK_WIDGET(create_group));
    gtk_widget_show(GTK_WIDGET(room_info));
    gtk_widget_show(GTK_WIDGET(stack_entry));
    gtk_stack_set_visible_child_name(stack_entry, "chat_message_entry");
    gtk_widget_show(GTK_WIDGET(btn_send_msg));
    gtk_widget_show(GTK_WIDGET(btn_send_sticker));
    gtk_widget_show_all(chat->listbox_dlgs);
    gtk_widget_show(GTK_WIDGET(btn_attach_file));
}

static void add_dialog_row(t_room *room, t_chat *chat, const gchar *group_name) {
    chat->listbox_dlgs = GTK_LIST_BOX(gtk_builder_get_object(chat->builder, "listbox_dlgs"));
    GtkWidget *row = gtk_list_box_row_new();
    char *id = my_itoa(room->room_id);
    GtkWidget *lbl = gtk_label_new(group_name);
    g_signal_connect(chat->listbox_dlgs, "row-selected", G_CALLBACK(select_room), chat);
     
    gtk_container_add(GTK_CONTAINER(chat->listbox_dlgs), row);
    gtk_widget_show(row);
    room->row_chat = row;
    gtk_container_add(GTK_CONTAINER(row), lbl);
    gtk_widget_show(lbl);
    g_object_set_data(G_OBJECT(row), "room", id);
    g_object_set_data(G_OBJECT(row), "room_name", id);
    gtk_widget_show_all(GTK_WIDGET(chat->listbox_dlgs));    
}

static void add_messages_box(t_room *room, t_chat *chat, const gchar *name) {
    manage_room_visibility(chat);
    GObject *stack = gtk_builder_get_object(chat->builder, "stack");
    GtkWidget *box = gtk_list_box_new();
    GtkWidget *scroll = gtk_scrolled_window_new(NULL, NULL);
    GtkWidget *view = gtk_viewport_new(NULL, NULL);
    char *id = my_itoa(room->room_id);
    GObject *stack_upper_dialog_toolbar = gtk_builder_get_object(chat->builder, "stack_upper_dialog_toolbar");

    gtk_container_add(GTK_CONTAINER(scroll), view);
    gtk_container_add(GTK_CONTAINER(view), box);
    room->listbox_msgs = GTK_LIST_BOX(box);
    g_signal_connect(box, "row-selected", G_CALLBACK(display_upper_panel), stack_upper_dialog_toolbar);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scroll), GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);
    gtk_stack_add_named(GTK_STACK(stack), scroll, (const gchar *)id);
    gtk_stack_set_visible_child(GTK_STACK(stack), scroll);
    room->stack = GTK_STACK(stack);
    room->scrll_wndw_msgs = GTK_SCROLLED_WINDOW(scroll);
    gtk_widget_show_all(GTK_WIDGET(scroll));
    g_object_set_data(G_OBJECT(scroll), "dialog", room);
}

static t_room *fill_room(int id, char *name, t_chat *chat) {
    t_room *room = (t_room *)malloc(sizeof(t_room));
    init_room(room, chat);

    room->room_id = i;
    room->chat_name = malloc(sizeof(const gchar) * strlen(name) + 1);
    strcpy(room->chat_name, name);
    i++;
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

void create_room(GtkButton *btn, t_chat *chat) {  
    GtkEntry *entry = GTK_ENTRY(gtk_builder_get_object(chat->builder, "group_name"));
    const gchar *name = gtk_entry_get_text(entry);
    
    //char *temp = send_rq_create_room_client(name, char **members);
    //enQueue(chat->config->queue_send, temp);
    t_room *room = fill_room(i, name, chat);
    add_messages_box(room, chat, name);
    add_dialog_row(room, chat, name);
    chat->curr_chat = room;
}

void req_create_dialog(GtkListBox *box, GtkListBoxRow *row, t_chat *chat) {
    const gchar *customer = gtk_widget_get_name(GTK_WIDGET(row));
    
    //char *temp = send_rq_create_room_client(chat->username, const gchar *customer);
    //enQueue(chat->config->queue_send, temp);
    printf("%s\n", customer);
    create_dialog(i, customer, chat);
    manage_visibility(box, chat);
}

void create_dialog(int id, char *name, t_chat *chat) {
    t_room *room = fill_room(i, name, chat);
    
    add_messages_box(room, chat, name);
    add_dialog_row(room, chat, name);
    chat->curr_chat = room;
}

static void clear_listbox_with_found_messages(GtkListBox *box) {
    GList *rows = NULL;
    GList *listrunner;

    if (gtk_container_get_children(GTK_CONTAINER(box)) != NULL) {
        rows = gtk_container_get_children(GTK_CONTAINER(box));
        listrunner = g_list_first(rows);    
        while (listrunner) {
            gtk_container_remove(box, listrunner->data);
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

    //clear_listbox_with_found_messages(box);
    gtk_widget_hide(GTK_WIDGET(box));
    gtk_widget_hide(GTK_WIDGET(lbl_local_search));
    gtk_widget_hide(GTK_WIDGET(lbl_local_search_nothing_found));
    gtk_widget_hide(GTK_WIDGET(lbl_global_search_nothing_found));
    gtk_widget_hide(GTK_WIDGET(lbl_global_search));
    gtk_entry_set_text(chat_search_user, ""); 
}
