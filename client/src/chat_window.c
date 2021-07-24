#include <gtk/gtk.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

GtkBuilder *builder;
GtkWidget *window;

struct LinkedList{
    gint id_room;
    GtkWidget *lbl;
    GtkWidget *chat_name;
    GtkWidget *wndw;
    GtkWidget *vwprt;
    GtkWidget *lst_bx;
    GtkWidget *r;
    GtkWidget *r1;
    struct LinkedList *next;
};

typedef struct LinkedList *node;//pointer to a linkedlist

typedef struct {
    //entries
    GtkEntry *chat_message_entry; 
    GtkEntry *chat_search_entry;
    //buttons
    GtkButton *btn_send_msg;
    GtkButton *btn_send_sticker;
    //stacks
    GtkStack *stack;
    GtkStack *stack_dialog_box;
    GtkStack *stack_upper_dialog_toolbar;

    //scrolled windows
    GtkScrolledWindow *scrll_wndw_msgs;
    GtkScrolledWindow *scrll_wndw_msgs3;
    GtkScrolledWindow *scrll_wndw_dlgs;
    GtkScrolledWindow *scrll_wndw_dlgs1;
    //listboxes
    GtkListBox *listbox_msgs;
    GtkListBox *listbox_dlgs;
    //widgets
    GtkWidget *item;
    GtkWidget *label_msg;
    GtkWidget *row;
    //labels
    GtkLabel *chat_receiver_name;
    GtkLabel *lbl_slct_chat;
    //GtkListBoxRow
    GtkListBoxRow *row_slctd;
    node head;
    node tmp;
} s_widgets;

node createNode(){
    node temp; // declare a node
    temp = (node)malloc(sizeof(struct LinkedList)); // allocate memory using for node
    temp->next = NULL;// make next point to NULL
    return temp;//return the new node
}   

gint ndx;
int i = 0;

void load_css_main(char *path);
void AddListItem(s_widgets *wdgts, char *sText);
void *get_time(s_widgets *wdgts);
void set_dialog_name (s_widgets *wdgts);
char *my_itoa(long long number);

int chat_window(int argc, char **argv) {
    s_widgets        *widgets = g_slice_new(s_widgets);
    // widgets->tmp->id_room = 0;

    gtk_init(&argc, &argv);
    load_css_main("client/styles/main.css");
    builder = gtk_builder_new();
    gtk_builder_add_from_file (builder, "client/glades/chat_window.glade", NULL);
    window = GTK_WIDGET(gtk_builder_get_object(builder, "chat_main"));

    //entries
    widgets->chat_search_entry = GTK_ENTRY(gtk_builder_get_object(builder, "chat_search_entry"));
    widgets->chat_message_entry = GTK_ENTRY(gtk_builder_get_object(builder, "chat_message_entry"));

    //stacks
    widgets->stack = GTK_STACK(gtk_builder_get_object(builder, "stack"));
    widgets->stack_dialog_box = GTK_STACK(gtk_builder_get_object(builder, "stack_dialog_box"));
    widgets->stack_upper_dialog_toolbar = GTK_STACK(gtk_builder_get_object(builder, "stack_upper_dialog_toolbar"));

    //scrolled windows
    widgets->scrll_wndw_msgs = GTK_SCROLLED_WINDOW(gtk_builder_get_object(builder, "scrll_wndw_msgs"));
    widgets->scrll_wndw_msgs3 = GTK_SCROLLED_WINDOW(gtk_builder_get_object(builder, "scrll_wndw_msgs3"));
    widgets->scrll_wndw_dlgs = GTK_SCROLLED_WINDOW(gtk_builder_get_object(builder, "scrll_wndw_dlgs"));
    widgets->scrll_wndw_dlgs1 = GTK_SCROLLED_WINDOW(gtk_builder_get_object(builder, "scrll_wndw_dlgs1"));

    //listboxes
    widgets->listbox_msgs = GTK_LIST_BOX(gtk_builder_get_object(builder, "listbox_msgs"));
    widgets->listbox_dlgs  = GTK_LIST_BOX(gtk_builder_get_object(builder, "listbox_dlgs"));

    //labels
    widgets->chat_receiver_name = GTK_LABEL(gtk_builder_get_object(builder, "chat_receiver_name"));

    //buttons 
    widgets->btn_send_msg = GTK_BUTTON(gtk_builder_get_object(builder, "btn_send_msg"));
    widgets->btn_send_sticker = GTK_BUTTON(gtk_builder_get_object(builder, "btn_send_sticker"));
    widgets->lbl_slct_chat = GTK_LABEL(gtk_builder_get_object(builder, "lbl_slct_chat"));

    gtk_builder_connect_signals(builder, widgets);
    g_object_unref(builder);

    gtk_widget_show_all(window);

    gtk_widget_hide(GTK_WIDGET(widgets->chat_message_entry));
    gtk_widget_hide(GTK_WIDGET(widgets->btn_send_msg));
    gtk_widget_hide(GTK_WIDGET(widgets->btn_send_sticker));

    gtk_main(); 
    g_slice_free(s_widgets, widgets); 
    return 0; 
}

void load_css_main(char *path) {
    GtkCssProvider *provider;
    GdkDisplay *display;
    GdkScreen *screen;

    provider = gtk_css_provider_new();
    display = gdk_display_get_default();
    screen = gdk_display_get_default_screen(display);

    const gchar *css_style_file = "client/styles/main.css";
    GFile *css_fp = g_file_new_for_path(css_style_file);
    GError *error = 0;

    gtk_style_context_add_provider_for_screen(screen, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    gtk_css_provider_load_from_file(provider, css_fp, &error);

    g_object_unref(provider);
}

void on_chat_main_destroy() {
    gtk_main_quit();
}

void AddListItem(s_widgets *wdgts, char *sText) {
    load_css_main("client/styles/messenger_window.css");
    wdgts->item = gtk_list_box_row_new();
    node cpy = createNode();
    cpy = wdgts->head;

    if (cpy != NULL) {
        while (cpy->next != NULL && gtk_stack_get_visible_child(wdgts->stack) != cpy->wndw)
                cpy = cpy->next;
    }
    gtk_list_box_insert(GTK_LIST_BOX(cpy->lst_bx), wdgts->item, -1); 
    gtk_widget_show(wdgts->item);
    wdgts->label_msg = gtk_label_new(sText);

    gtk_container_add(GTK_CONTAINER(wdgts->item), wdgts->label_msg);
    gtk_list_box_row_set_selectable((GtkListBoxRow *)wdgts->item, TRUE); 
    gtk_widget_set_halign(wdgts->label_msg, GTK_ALIGN_END);
    gtk_widget_show (wdgts->label_msg);
}

void on_btn_send_msg_clicked(GtkButton *chat_send_btn, s_widgets *wdgts) {
    char buffer[1024];

    sprintf(buffer, "%s", gtk_entry_get_text(wdgts->chat_message_entry));

    if (strlen(buffer) > 0) {
        //inserting '\n' into the message after each 45 symbols
        if (strlen(buffer) > 45) {
            char new_buffer[strlen(buffer) + (strlen(buffer) % 45)];
            int i = 0;

            while (i < strlen(buffer)) {
                if (i % 45 == 0) {
                    new_buffer[i] = '\n';
                    i++;
                }
                else {
                    new_buffer[i] = buffer[i];
                    i++;
                }
            }              
            AddListItem(wdgts, new_buffer);    
        } else {
            AddListItem(wdgts, buffer);
        }
        get_time(wdgts); 
        gtk_entry_set_text(wdgts->chat_message_entry,""); 
    }
    printf("text from entry:%s\n", (const gchar*) buffer);
}

void on_chat_message_entry_activate(GtkEntry *chat_message_entry, s_widgets *wdgts) {
    char buffer[1024];

    sprintf(buffer, "%s", gtk_entry_get_text(wdgts->chat_message_entry));
    if (strlen(buffer) > 0) {
        if (strlen(buffer) > 45) {
            char new_buffer[strlen(buffer) + (strlen(buffer) % 45)];
            int i = 0;

            while (i < strlen(buffer)) {
                if (i % 45 == 0) {
                    new_buffer[i] = '\n';
                    i++;
                }
                else {
                    new_buffer[i] = buffer[i];
                    i++;
                }
            }              
            AddListItem(wdgts, new_buffer); 
        } else {
            AddListItem(wdgts, buffer);
        }
        get_time(wdgts); 
        gtk_entry_set_text(wdgts->chat_message_entry,""); 
    }
}

void *get_time(s_widgets *wdgts) {
    load_css_main("client/styles/main.css");
    GDateTime   *time;            // for storing current time and date
    gchar       *time_str;        // current time and date as a string
    
    time     = g_date_time_new_now_local();             // get the current time
    time_str = g_date_time_format(time, "%H:%M:%S");    // convert current time to string
    
    wdgts->row = gtk_list_box_row_new();
    gtk_list_box_row_set_selectable((GtkListBoxRow *)wdgts->row, FALSE);    

    node cpy = createNode();
    cpy = wdgts->head;

    if (cpy != NULL) {
        while (cpy->next != NULL && gtk_stack_get_visible_child(wdgts->stack) != cpy->wndw)
            cpy = cpy->next;
    }

    gtk_list_box_insert (GTK_LIST_BOX(cpy->lst_bx), wdgts->row, -1); 
    
    wdgts->label_msg = gtk_label_new(time_str);
    gtk_label_set_selectable((GtkLabel *)wdgts->label_msg, FALSE);

    gtk_container_add(GTK_CONTAINER(wdgts->row), wdgts->label_msg);
    gtk_widget_set_halign(wdgts->label_msg, GTK_ALIGN_END);
    gtk_widget_show (wdgts->label_msg);

    g_free(time_str);
    g_date_time_unref(time);
    gtk_widget_show (wdgts->row);
}

void on_chat_settings_settings_btn_clicked(GtkButton *chat_settings_settings_btn, GtkStack *stack_dialog_box) {
    g_return_if_fail(GTK_IS_BUTTON(chat_settings_settings_btn));
    g_return_if_fail(GTK_IS_STACK(stack_dialog_box));

    if (strcmp(gtk_stack_get_visible_child_name(stack_dialog_box), "scrll_wndw_dlgs1") == 0) 
        gtk_stack_set_visible_child_name (stack_dialog_box, "scrll_wndw_dlgs");    
    else if (strcmp(gtk_stack_get_visible_child_name(stack_dialog_box), "scrll_wndw_dlgs") == 0)  
        gtk_stack_set_visible_child_name (stack_dialog_box, "scrll_wndw_dlgs1");
}

void on_btn_cancel_clicked(GtkButton *btn_cancel, GtkStack *stack_upper_dialog_toolbar) {
    g_return_if_fail(GTK_IS_BUTTON(btn_cancel));
    g_return_if_fail(GTK_IS_STACK(stack_upper_dialog_toolbar));

    gtk_stack_set_visible_child_full (stack_upper_dialog_toolbar, "chat_topbar_grid", 0);
}

void on_listbox_msgs_row_selected(GtkListBox *box, GtkListBoxRow *row, GtkStack *stack_upper_dialog_toolbar, s_widgets *wdgts) {
    g_return_if_fail (GTK_IS_LIST_BOX (box));
    g_return_if_fail (row == NULL || GTK_IS_LIST_BOX_ROW (row));

    node cpy = createNode();
    cpy = wdgts->head;
    
    if (cpy != NULL) {
        while (cpy->next != NULL && gtk_stack_get_visible_child(wdgts->stack) != cpy->wndw) {
                cpy = cpy->next;
        } 
    }

    box = GTK_LIST_BOX(cpy->lst_bx);
    row = GTK_LIST_BOX_ROW(cpy->r);

    ndx = gtk_list_box_row_get_index(row);
    printf("%d\n", ndx);

    if (cpy->r && gtk_list_box_row_get_selectable(GTK_LIST_BOX_ROW(cpy->r)) == TRUE)
        gtk_stack_set_visible_child_name (stack_upper_dialog_toolbar, "chat_topbar_wth_btns");
    if (cpy->r && gtk_list_box_row_get_selectable(GTK_LIST_BOX_ROW(cpy->r)) == FALSE)
        gtk_stack_set_visible_child_name(stack_upper_dialog_toolbar, "chat_topbar_grid");
}

void gtk_container_foreach (GtkContainer *container, GtkCallback callback, gpointer callback_data) {

}

void on_btn_edit_msg_clicked(GtkButton *btn_edit_msg, s_widgets *wdgts) {
}

void on_btn_delete_msg_clicked(GtkButton *btn_delete_msg, s_widgets *wdgts) {
    node cpy = createNode();
    cpy = wdgts->head;

    if (cpy != NULL) {
        while (cpy->next != NULL && gtk_stack_get_visible_child(wdgts->stack) != cpy->wndw)
            cpy = cpy->next;
    }
    int indx = gtk_list_box_row_get_index(gtk_list_box_get_selected_row((GtkListBox *)cpy->lst_bx));
    //gtk_container_remove((GtkContainer *)cpy->lst_bx, (GtkWidget *)gtk_list_box_get_selected_row((GtkListBox *)cpy->lst_bx));
    gtk_container_remove((GtkContainer *)cpy->lst_bx, (GtkWidget *)gtk_list_box_get_row_at_index((GtkListBox *)cpy->lst_bx, indx));
    gtk_container_remove((GtkContainer *)cpy->lst_bx, (GtkWidget *)gtk_list_box_get_row_at_index((GtkListBox *)cpy->lst_bx, indx));
}

char *my_itoa(long long number) {
    char *str = (char *)malloc(21);
	int count = 0;
	unsigned long long num;
    int flag = 0;

	if(number & 0x8000000000000000) {
        flag = 1;
		str[count++] = '-';
		num = (number ^ 0xFFFFFFFFFFFFFFFF) + 1;
	}
	else
		num = (unsigned long long) number;
	if(num == 0)
		str[count++] = '0';
	for(; num != 0; num /= 10)
		str[count++] = ((num % 10) + '0');
    for(int i = 0; (i + flag) < (count + flag) / 2; ++i) {
            char temp = str[i + flag];
            str[i + flag] = str[count - i - 1];
            str[count - i - 1] = temp;
        }
    str[count] = '\0';
    return str;
}

void on_chat_settings_terminal_btn_clicked(GtkButton *chat_settings_terminal_btn, s_widgets *wdgts) {
    node cpy = createNode();
    cpy = wdgts->head;
    if (cpy == NULL) {
    }
    else { 
        while (cpy->next != NULL) {
                cpy = cpy->next;
            if (cpy->id_room == 4) {
                g_print("\n");
                gtk_stack_set_visible_child(wdgts->stack, (GtkWidget *)cpy->wndw);
                g_print("Switching to %s.\n", gtk_stack_get_visible_child_name (wdgts->stack));
            }
        } 
    }
}

void push_node(s_widgets *wdgts) {
    if (wdgts->tmp == NULL) {
        wdgts->tmp = createNode();
        printf("Created a first node\n"); 
    }
    else {  
        while (wdgts->tmp->next != NULL) {
            g_print("check room id in first cycle : %d\n", wdgts->head->id_room);

            wdgts->tmp = wdgts->tmp->next;
        }
        wdgts->tmp->next = createNode();
        printf("Created a node\n");
    }
}

void mx_print(GtkListBox *box, GtkListBoxRow *r, GtkStack *stack_upper_dialog_toolbar) {
    if (r) {
        if (gtk_list_box_row_get_selectable(r) == TRUE)
            gtk_stack_set_visible_child_name (stack_upper_dialog_toolbar, "chat_topbar_wth_btns");
        if (gtk_list_box_row_get_selectable(r) == FALSE)
            gtk_stack_set_visible_child_name(stack_upper_dialog_toolbar, "chat_topbar_grid");
    }
}

void on_chat_settings_users_btn_clicked(GtkButton *btn, s_widgets *wdgts) {
    load_css_main("client/styles/messenger_window.css");
    // gtk_widget_destroy(GTK_WIDGET(wdgts->lbl_slct_chat));
    gtk_widget_show(GTK_WIDGET(wdgts->btn_send_msg));
    gtk_widget_show(GTK_WIDGET(wdgts->btn_send_sticker));
    gtk_widget_show(GTK_WIDGET(wdgts->chat_message_entry));    
    push_node(wdgts);

    builder = gtk_builder_new();
    wdgts->tmp->id_room = i;
    printf("i: %d\nroom_id: %d\n",i, wdgts->tmp->id_room);

    wdgts->tmp->r1 = gtk_list_box_row_new();
    gtk_list_box_insert (wdgts->listbox_dlgs, wdgts->tmp->r1, -1);
    gtk_widget_show((GtkWidget *)wdgts->tmp->r1);

    wdgts->tmp->chat_name = gtk_label_new(my_itoa(i));
    gtk_container_add(GTK_CONTAINER(wdgts->tmp->r1), (GtkWidget *)wdgts->tmp->chat_name);
    gtk_widget_show((GtkWidget *)wdgts->tmp->chat_name);

    wdgts->tmp->wndw = gtk_scrolled_window_new(NULL, NULL);
    gtk_container_add(GTK_CONTAINER(wdgts->stack), GTK_WIDGET(wdgts->tmp->wndw));
    gtk_widget_show((GtkWidget *)wdgts->tmp->wndw);

    wdgts->tmp->vwprt = gtk_viewport_new(NULL, NULL);
    gtk_container_add(GTK_CONTAINER(wdgts->tmp->wndw), GTK_WIDGET(wdgts->tmp->vwprt));
    gtk_widget_show((GtkWidget *)wdgts->tmp->vwprt);

    wdgts->tmp->lst_bx = gtk_list_box_new();
    gtk_container_add(GTK_CONTAINER(wdgts->tmp->vwprt), GTK_WIDGET(wdgts->tmp->lst_bx));
    gtk_widget_show((GtkWidget *)wdgts->tmp->lst_bx);
    
    g_signal_connect(wdgts->tmp->lst_bx, "row-selected", G_CALLBACK(mx_print), wdgts->stack_upper_dialog_toolbar);

    gtk_stack_set_visible_child(wdgts->stack, (GtkWidget *)wdgts->tmp->wndw);
    g_print("Switching to %s.\n", gtk_stack_get_visible_child_name (wdgts->stack));

    wdgts->tmp->r = gtk_list_box_row_new();
    gtk_list_box_insert (GTK_LIST_BOX(wdgts->tmp->lst_bx), wdgts->tmp->r, -1); 
    gtk_widget_show (wdgts->tmp->r);
    
    wdgts->tmp->lbl = gtk_label_new(my_itoa(i));
    gtk_container_add(GTK_CONTAINER(wdgts->tmp->r), wdgts->tmp->lbl);
    gtk_list_box_row_set_selectable((GtkListBoxRow *)wdgts->tmp->r, TRUE); 
    gtk_widget_set_halign(wdgts->tmp->lbl, GTK_ALIGN_END);
    gtk_widget_show (wdgts->tmp->lbl);

    if (wdgts->tmp->next == NULL) {
        printf("entered the cycle\n");
        wdgts->head = createNode();
        wdgts->head = wdgts->tmp;
        g_print("head room id is equiled to: %d\n", wdgts->head->id_room);
    }
    i++;
}

void on_listbox_dlgs_row_selected(GtkListBox *box, GtkListBoxRow *row, s_widgets *wdgts) {
    node cpy = createNode();
    cpy = wdgts->head;

    if (cpy != NULL) {
        while (cpy->next != NULL && gtk_list_box_row_get_index(row) != cpy->id_room) 
            cpy = cpy->next;
    }   
    gtk_stack_set_visible_child(wdgts->stack, cpy->wndw);
}
