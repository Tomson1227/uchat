#include "uchat.h"

static void req_log_in(GtkButton *btn, t_chat *chat);
static void req_sign_up(GtkButton *btn, t_chat *chat);
static void connect_sign_in_up(t_chat *chat);
static void on_btn_sign_up_clicked(GtkButton *btn_sign_up, t_chat *chat); 
static void init_main_window(t_chat *chat);

void init_gui(t_chat *chat) {
    connect_sign_in_up(chat);
    connect_chat(chat);
    connect_room_settings(chat);
    connect_profile_settings(chat);
    connect_search_username_room(chat);
    
    init_main_window(chat);
}

static void init_main_window(t_chat *chat) {
    GObject *wnd_main = gtk_builder_get_object(chat->builder, "window_main");

    gtk_widget_show(GTK_WIDGET(wnd_main));
    load_css("client/styles/login_signup.css");
}

static void on_btn_sign_up_clicked(GtkButton *btn_sign_up, t_chat *chat) {
    GtkStack *stack = GTK_STACK(gtk_builder_get_object(chat->builder, "stac"));
    GtkEntry *username = GTK_ENTRY(gtk_builder_get_object(chat->builder, "username"));
    GtkEntry *password = GTK_ENTRY(gtk_builder_get_object(chat->builder, "password"));

    gtk_entry_set_text(username, "");
    gtk_entry_set_text(password, "");
    gtk_stack_set_visible_child_full (stack, "sign_up", 0);
}

void on_window_main_destroy() {
    gtk_main_quit();
}

void on_btn_back_console_clicked(GtkButton *btn_back_console, GtkStack *stack) {
    gtk_stack_set_visible_child_full (stack, "menu", 0);
    // load_css("client/styles/main_menu.css");
}

void on_btn_gui_clicked(GtkButton *btn_gui_clicked, GtkStack *stack) {
    gtk_stack_set_visible_child_full (stack, "log_in", 0);
    // load_css("client/styles/login_signup.css");
}

void on_btn_console_clicked(GtkButton *btn_console_clicked, GtkStack *stack) {
    gtk_stack_set_visible_child_full(stack, "console", 0);
    // load_css("client/styles/console.css");
}

void on_btn_back_log_in_clicked(GtkButton *btn_back_log_in, GtkStack *stack) {
     gtk_stack_set_visible_child_full (stack, "menu", 0);
    //  load_css("client/styles/main_menu.css");
}

void on_btn_back_clicked(GtkButton *btn_back, GtkStack *stack) {
    g_return_if_fail(GTK_IS_BUTTON(btn_back));
    g_return_if_fail(GTK_IS_STACK(stack));
    gtk_stack_set_visible_child_full (stack, "log_in", 0);
    // load_css("client/styles/login_signup.css");
}

static void req_log_in(GtkButton *btn, t_chat *chat) {
    GtkLabel *label = GTK_LABEL(gtk_builder_get_object(chat->builder, "lbl_error_log_in"));
    char buf_username[40];
    char buf_password[40];
    GtkEntry *username = GTK_ENTRY(gtk_builder_get_object(chat->builder, "username"));
    GtkEntry *password = GTK_ENTRY(gtk_builder_get_object(chat->builder, "password"));

    sprintf(buf_username, "%s", gtk_entry_get_text(username));
    sprintf(buf_password, "%s", gtk_entry_get_text(password)); 
    if (strlen(buf_password) == 0 || strlen(buf_username) == 0 || (strlen(buf_password) == 0 && strlen(buf_username) == 0))
        gtk_label_set_text(GTK_LABEL(label), "please enter your credentials");
    else if (strlen(buf_username) < 6 || strlen(buf_password) < 6) {
        gtk_label_set_text(GTK_LABEL(label), "credentials should inlcude\n   more than 6 symbols");
    }
    else {
        gtk_label_set_text(GTK_LABEL(label), "");
        chat->username = malloc(sizeof(char) * strlen(buf_username));
        strcpy(chat->username, buf_username);
        char *temp = (char*)calloc(128, sizeof(char));
        temp = send_rq_log_in_client(buf_username, buf_password);
        enQueue(chat->config->queue_send, temp);
        while(QueueisEmpty(chat->config->queue_recv)) {
        }
        char *dq = deQueue(chat->config->queue_recv);
        printf("%s\n", dq);
        process_rs_client(dq, chat);
    }
}

static void req_sign_up(GtkButton *btn, t_chat *chat) {
    GtkLabel *label = GTK_LABEL(gtk_builder_get_object(chat->builder, "lbl_error_display"));
    char buf_username[40];
    char buf_password[40];
    char buf_password_repeated[40];

    GtkEntry *username = GTK_ENTRY(gtk_builder_get_object(chat->builder, "username_sign_up"));
    GtkEntry *password = GTK_ENTRY(gtk_builder_get_object(chat->builder, "password_sign_up"));
    GtkEntry *password_repeated = GTK_ENTRY(gtk_builder_get_object(chat->builder, "repeat_password_sign_up"));

    sprintf(buf_username, "%s", gtk_entry_get_text(username));
    sprintf(buf_password, "%s", gtk_entry_get_text(password)); 
    sprintf(buf_password_repeated, "%s", gtk_entry_get_text(password_repeated));
    if (strlen(buf_username) == 0 || strlen(buf_password) == 0 || strlen(buf_password_repeated) == 0)
        gtk_label_set_text(GTK_LABEL(label), "please enter your credentials");
    else if (strcmp(buf_password, buf_password_repeated) != 0) 
        gtk_label_set_text(GTK_LABEL(label), "passwords do not match");
    else if (strcmp(buf_password, buf_password_repeated) == 0 && strlen(buf_password) < 6)
        gtk_label_set_text(GTK_LABEL(label), "password should inlcude\n   more than 6 symbols");
    else 
    {
        gtk_label_set_text(GTK_LABEL(label), "");
        chat->username = malloc(sizeof(char) * strlen(buf_username));//move to process_rs_client function
        strcpy(chat->username, buf_username);
        char *tmp = send_rq_sign_in_client(buf_username, buf_password);
        enQueue(chat->config->queue_send, tmp);
        while(QueueisEmpty(chat->config->queue_recv)) {
        }
        char *dq = deQueue(chat->config->queue_recv);
        process_rs_client(dq, chat);
    }
}

void change_password_visibility(GtkEntry *entry) {
    if (gtk_entry_get_visibility(entry) == TRUE)
        gtk_entry_set_visibility(entry, FALSE);
    else 
        gtk_entry_set_visibility(entry, TRUE);
}

static void connect_sign_in_up(t_chat *chat) {
    GtkButton *btn_log_in = GTK_BUTTON(gtk_builder_get_object(chat->builder, "btn_log_in"));
    GtkButton *btn_sign_up = GTK_BUTTON(gtk_builder_get_object(chat->builder, "btn_sign_up_sign_up"));
    GtkButton *btn_sign = GTK_BUTTON(gtk_builder_get_object(chat->builder, "btn_sign_up"));
    GtkEntry *password = GTK_ENTRY(gtk_builder_get_object(chat->builder, "password"));

    g_signal_connect(password, "icon-press", G_CALLBACK(change_password_visibility), NULL);     
    g_signal_connect(btn_log_in, "clicked", G_CALLBACK(req_log_in), chat);
    g_signal_connect(btn_sign_up, "clicked", G_CALLBACK(req_sign_up), chat);
    g_signal_connect(btn_sign, "clicked", G_CALLBACK(on_btn_sign_up_clicked), chat);
}

gint start_gui(t_chat *chat) {
    gtk_main();
    return 0;
}

void on_password_sign_up_icon_press(GtkEntry *entry, GtkEntryIconPosition GTK_ENTRY_ICON_SECONDARY, GdkEvent *event, GtkEntry *repeat_password) {
    if (gtk_entry_get_visibility(entry) == TRUE) {
        gtk_entry_set_visibility(entry, FALSE);
        gtk_entry_set_visibility(repeat_password, FALSE);
    }
    else {
        gtk_entry_set_visibility(entry, TRUE);
        gtk_entry_set_visibility(repeat_password, TRUE);
    } 
}
