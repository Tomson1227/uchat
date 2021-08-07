#include "uchat.h"

int i = 0;

static t_room *fill_room(char *rs, t_chat *chat);
static void manage_room_visibility(t_chat *chat); 
static void add_dialog_row(t_room *room, t_chat *chat);
static void add_messages_box(t_room *room, t_chat *chat);
static void init_room(t_room *room, t_chat *chat);

void display_upper_panel(GtkListBox *box, GtkListBoxRow *r, GtkStack *stack_upper_dialog_toolbar) {
    // int id = g_object_get_data(r, "msg_id");
    // printf("selected msg with id: %d\n", id);

    if (r) {
        if (gtk_list_box_row_get_selectable(r) == TRUE)
            gtk_stack_set_visible_child_name (stack_upper_dialog_toolbar, "chat_topbar_wth_btns");
    }
}

void select_room(GtkListBox *box, GtkListBoxRow *row, t_chat *chat) {
    manage_room_visibility(chat);
    char *tmp = g_object_get_data(G_OBJECT(row), "room");
    GtkStack *stack = GTK_STACK(gtk_builder_get_object(chat->builder, "stack"));
    GtkWidget *scroll = gtk_stack_get_child_by_name(stack, tmp);
    gtk_stack_set_visible_child_name(stack, tmp);

    t_room *dialog = g_object_get_data(G_OBJECT(scroll),"dialog");
    chat->curr_chat = dialog;
    printf("switched to room: %d\n", chat->curr_chat->room_id);
}

static void manage_room_visibility(t_chat *chat) {
    GtkEntry *chat_message_entry = GTK_ENTRY(gtk_builder_get_object(chat->builder, "chat_message_entry"));
    GtkButton *btn_send_msg = GTK_BUTTON(gtk_builder_get_object(chat->builder, "btn_send_msg"));
    GtkButton *btn_send_sticker = GTK_BUTTON(gtk_builder_get_object(chat->builder, "btn_send_sticker"));
    GtkButton *room_info = GTK_BUTTON(gtk_builder_get_object(chat->builder, "room_info"));
    GtkButton *btn_attach_file = GTK_BUTTON(gtk_builder_get_object(chat->builder, "btn_attach_file"));
    
    gtk_widget_show(GTK_WIDGET(room_info));
    gtk_widget_show(GTK_WIDGET(btn_send_msg));
    gtk_widget_show(GTK_WIDGET(btn_send_sticker));
    gtk_widget_show(GTK_WIDGET(chat_message_entry));
    gtk_widget_show(GTK_WIDGET(btn_attach_file));
}

static void add_dialog_row(t_room *room, t_chat *chat) {
    chat->listbox_dlgs = GTK_LIST_BOX(gtk_builder_get_object(chat->builder, "listbox_dlgs"));
    GtkWidget *row = gtk_list_box_row_new();
    // GtkWidget *label = gtk_label_new(room->chat_name);
    char *id = my_itoa(room->room_id);
    GtkWidget *lbl = gtk_label_new(id);
    g_signal_connect(chat->listbox_dlgs, "row-selected", G_CALLBACK(select_room), chat);
     
    gtk_container_add(GTK_CONTAINER(chat->listbox_dlgs), row);
    gtk_widget_show(row);
    room->row_chat = row;
    gtk_container_add(GTK_CONTAINER(row), lbl);
    gtk_widget_show(lbl);
    g_object_set_data(G_OBJECT(row), "room", id);
    gtk_widget_show_all(GTK_WIDGET(chat->listbox_dlgs));    
}

static void add_messages_box(t_room *room, t_chat *chat) {
    manage_room_visibility(chat);
    GObject *stack = gtk_builder_get_object(chat->builder, "stack");
    GtkWidget *box = gtk_list_box_new();
    GtkWidget *scroll = gtk_scrolled_window_new(NULL, NULL);
    GtkWidget *view = gtk_viewport_new(NULL, NULL);
    char *id = my_itoa(room->room_id);
    GtkWidget *lbl = gtk_label_new("this is your very beginning of talk with ");
    GtkWidget *r = gtk_list_box_row_new();
    GObject *stack_upper_dialog_toolbar = gtk_builder_get_object(chat->builder, "stack_upper_dialog_toolbar");

    gtk_container_add(GTK_CONTAINER(scroll), view);
    gtk_container_add(GTK_CONTAINER(view), box);
    room->listbox_msgs = GTK_LIST_BOX(box);
    g_signal_connect(box, "row-selected", G_CALLBACK(display_upper_panel), stack_upper_dialog_toolbar);
    // gtk_list_box_set_selection_mode(room->listbox_msgs, GTK_SELECTION_MULTIPLE);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scroll), GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);
    gtk_widget_show(scroll);
    gtk_stack_add_named(GTK_STACK(stack), scroll, (const gchar *)id);
    gtk_stack_set_visible_child(GTK_STACK(stack), scroll);
    room->stack = GTK_STACK(stack);
    room->scrll_wndw_msgs = GTK_SCROLLED_WINDOW(scroll);
    gtk_container_add(GTK_CONTAINER(box), r);
    gtk_widget_show(box);
    gtk_widget_show(r);
    gtk_list_box_row_set_selectable(r, FALSE);
    gtk_container_add(GTK_CONTAINER(r), lbl);
    gtk_widget_set_halign(lbl, GTK_ALIGN_CENTER);
    gtk_widget_show(lbl);
    gtk_widget_show_all(scroll);
    g_object_set_data(G_OBJECT(scroll), "dialog", room);
}

static t_room *fill_room(char *rs, t_chat *chat) {
    t_room *room = (t_room *)malloc(sizeof(t_room));
    init_room(room, chat);
    
    room->room_id = i;
    //appoint id_room
    //appoint chat_name
    //appoint customer_id
    i++;
    return room;
} 

static void init_room(t_room *room, t_chat *chat) {
    room->stack = NULL;
    room->scrll_wndw_msgs = NULL;
    room->vwprt = NULL;
    room->listbox_msgs = NULL;
    room->row_chat = NULL;
    room->row_index = -1;
    room->lbl = NULL;
    room->chat_name = NULL;
    room->room_id = -1;
    room->members = NULL;
}

void create_room(GtkButton *btn, t_chat *chat) {
    //char *temp = (char*)calloc(128, sizeof(char));
    //temp = send_rq_create_room_client(chat->username, char *customer);
    //char *dq = deQueue(config->queue_send);
    //
    //process_rs_client(dq, t_chat *chat);

    t_room *room = fill_room(NULL, chat);
    printf("i: %d\nroom_id: %d\n", i, room->room_id);
    add_messages_box(room, chat);
    add_dialog_row(room, chat);
    chat->curr_chat = room;
}
