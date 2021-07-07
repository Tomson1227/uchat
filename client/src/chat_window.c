#include <gtk/gtk.h>

typedef struct 
{
    //entries
    GtkEntry *chat_message_entry; 
    GtkEntry *chat_search_entry;
    //buttons
    GtkButton *chat_send_btn;
    GtkButton *chat_settings_settings_btn;
    GtkButton *chat_settings_users_btn;
    GtkButton *chat_settings_terminal_btn;
    GtkButton *btn_svd_msgs;
    GtkButton *btn_dialog1; 
    //stacks
    GtkStack *stack;
    GtkStack *stack_dialog_box;
    //scrolled windows
    GtkScrolledWindow *scrll_wndw_msgs;
    GtkScrolledWindow *scrll_wndw_msgs3;
    GtkScrolledWindow *scrll_wndw_dlgs;
    //listboxes
    GtkListBox *listbox_msgs;
    GtkListBox *listbox_msgs2;
    GtkListBox *listbox_dlgs;
    //widgets
    GtkWidget *item;
    GtkWidget *label_msg;
    //viewports
    GtkViewport *viewport;
    GtkViewport *viewport1;
    GtkViewport *viewport_dlgs;

    //labels
    GtkLabel *chat_receiver_name;

} s_widgets;

void load_css_main(char *path);
void AddListItem (s_widgets *wdgts, char *sText);

void set_dialog_name (s_widgets *wdgts);
int chat_window(int argc, char **argv)
{
    GtkBuilder *builder;
    GtkWidget *window;
    s_widgets        *widgets = g_slice_new(s_widgets);
    gtk_init(&argc, &argv);

    load_css_main("client/styles/main.css");

    builder = gtk_builder_new();
    gtk_builder_add_from_file (builder, "client/glades/chat_window.glade", NULL);

    window = GTK_WIDGET(gtk_builder_get_object(builder, "chat_main"));
    //entries
    widgets->chat_search_entry = GTK_ENTRY(gtk_builder_get_object(builder, "chat_search_entry"));
    widgets->chat_message_entry = GTK_ENTRY(gtk_builder_get_object(builder, "chat_message_entry"));

    //buttons
    widgets->chat_send_btn = GTK_BUTTON(gtk_builder_get_object(builder, "chat_send_btn"));
    widgets->chat_settings_settings_btn = GTK_BUTTON(gtk_builder_get_object(builder, "chat_settings_settings_btn"));
    widgets->chat_settings_users_btn = GTK_BUTTON(gtk_builder_get_object(builder, "chat_settings_users_btn"));
    widgets->chat_settings_terminal_btn = GTK_BUTTON(gtk_builder_get_object(builder, "chat_settings_terminal_btn"));
    widgets->btn_svd_msgs =  GTK_BUTTON(gtk_builder_get_object(builder, "btn_svd_msgs"));
    widgets->btn_dialog1 =   GTK_BUTTON(gtk_builder_get_object(builder, "btn_dialog1"));
    //stacks
    widgets->stack = GTK_STACK(gtk_builder_get_object(builder, "stack"));
    widgets->stack_dialog_box = GTK_STACK(gtk_builder_get_object(builder, "stack_dialog_box"));

    //viewports
    widgets->viewport = GTK_VIEWPORT(gtk_builder_get_object(builder, "viewport"));
    widgets->viewport1 = GTK_VIEWPORT(gtk_builder_get_object(builder, "viewport1"));
    widgets->viewport_dlgs  = GTK_VIEWPORT(gtk_builder_get_object(builder, "viewport_dlgs"));
    //widgets
    widgets->item = GTK_WIDGET(gtk_builder_get_object(builder, "item"));
    widgets->label_msg = GTK_WIDGET(gtk_builder_get_object(builder, "label_msg"));

    //scrolled windows
    widgets->scrll_wndw_msgs = GTK_SCROLLED_WINDOW(gtk_builder_get_object(builder, "scrll_wndw_msgs"));
    widgets->scrll_wndw_msgs3 = GTK_SCROLLED_WINDOW(gtk_builder_get_object(builder, "scrll_wndw_msgs3"));
    widgets->scrll_wndw_dlgs = GTK_SCROLLED_WINDOW(gtk_builder_get_object(builder, "scrll_wndw_dlgs"));

    //listboxes
    widgets->listbox_msgs = GTK_LIST_BOX(gtk_builder_get_object(builder, "listbox_msgs"));
    widgets->listbox_msgs2 = GTK_LIST_BOX(gtk_builder_get_object(builder, "listbox_msgs2"));
    widgets->listbox_dlgs  = GTK_LIST_BOX(gtk_builder_get_object(builder, "listbox_dlgs"));

    //labels
    widgets->chat_receiver_name = GTK_LABEL(gtk_builder_get_object(builder, "chat_receiver_name"));
    //set_dialog_name (widgets);   
    gtk_builder_connect_signals(builder, widgets);
    g_object_unref(builder);

    gtk_widget_show_all(window);
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

void on_chat_send_btn_clicked(GtkButton *chat_send_btn, s_widgets *wdgts) {
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
        gtk_entry_set_text(wdgts->chat_message_entry,""); 
    }
    printf("text from entry:%s\n", (const gchar*) buffer);
}

void on_chat_main_destroy() {
    gtk_main_quit();
}

void AddListItem (s_widgets *wdgts, char *sText)
{
    load_css_main("client/styles/messenger_window.css");
    wdgts->item = gtk_list_box_row_new();
    
    gtk_list_box_insert (wdgts->listbox_msgs, wdgts->item, -1); 
    gtk_widget_show (wdgts->item);
    wdgts->label_msg = gtk_label_new(sText);
    gtk_container_add(GTK_CONTAINER(wdgts->item), wdgts->label_msg);
    gtk_widget_set_halign(wdgts->label_msg, GTK_ALIGN_END);
    gtk_widget_show (wdgts->label_msg);
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
                    printf("entered the cycle\n");
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
        gtk_entry_set_text(wdgts->chat_message_entry,""); 
    }
}

void on_btn_svd_msgs_clicked(GtkButton *btn_svd_msgs, GtkStack *stack) {
    g_return_if_fail(GTK_IS_BUTTON(btn_svd_msgs));
    g_return_if_fail(GTK_IS_STACK(stack));
    gtk_stack_set_visible_child_full (stack, "scrll_wndw_msgs", 0);
    g_print("Switching to %s.\n", gtk_stack_get_visible_child_name (stack));
}

void on_btn_dialog1_clicked(GtkButton *btn_dialog1, GtkStack *stack, s_widgets *wdgts) {
    g_return_if_fail(GTK_IS_BUTTON(btn_dialog1));
    g_return_if_fail(GTK_IS_STACK(stack));
    gtk_stack_set_visible_child_full (stack, "scrll_wndw_msgs3", 0);
    g_print("Switching to %s.\n", gtk_stack_get_visible_child_name (stack));
} 

void set_dialog_name (s_widgets *wdgts) {
    gtk_label_set_text (wdgts->chat_receiver_name, "alisa");
}