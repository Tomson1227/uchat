#include <gtk/gtk.h>

void load_css_main() {
    GtkCssProvider *provider;
    GdkDisplay *display;
    GdkScreen *screen;

    const gchar *css_style_file = "client/styles/main.css";
    GFile *css_fp = g_file_new_for_path(css_style_file);
    GError *error = NULL;

    provider = gtk_css_provider_new();
    display = gdk_display_get_default();
    screen = gdk_display_get_default_screen(display);

    gtk_style_context_add_provider_for_screen(screen, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    gtk_css_provider_load_from_file(provider, css_fp, &error);

    g_object_unref(provider);
}

int chat_window(int argc, char **argv)
{
    GtkBuilder *builder;
    GtkWidget *window;
    GtkWidget *chat_search_entry;
    GtkWidget *chat_message_entry;
    GtkWidget *chat_send_btn;
    GtkWidget *chat_settings_settings_btn;
    GtkWidget *chat_settings_users_btn;
    GtkWidget *chat_settings_terminal_btn;
    GtkCssProvider *provider = NULL;

    gtk_init(&argc, &argv);

    load_css_main();
    gtk_css_provider_load_from_path(provider, "client/styles/main.css", NULL);

    builder = gtk_builder_new();
    gtk_builder_add_from_file (builder, "client/glades/chat_window.glade", NULL);

    window = GTK_WIDGET(gtk_builder_get_object(builder, "chat_main"));
    chat_search_entry = GTK_WIDGET(gtk_builder_get_object(builder, "chat_search_entry"));
    chat_message_entry = GTK_WIDGET(gtk_builder_get_object(builder, "chat_message_entry"));
    chat_send_btn = GTK_WIDGET(gtk_builder_get_object(builder, "chat_send_btn"));
    chat_settings_settings_btn = GTK_WIDGET(gtk_builder_get_object(builder, "chat_settings_settings_btn"));
    chat_settings_users_btn = GTK_WIDGET(gtk_builder_get_object(builder, "chat_settings_users_btn"));
    chat_settings_terminal_btn = GTK_WIDGET(gtk_builder_get_object(builder, "chat_settings_terminal_btn"));
    gtk_builder_connect_signals(builder, NULL);

    g_object_unref(builder);

    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}

// called when window is closed
void on_chat_main_destroy()
{
    gtk_main_quit();
}

void on_chat_send_btn_clicked() {
    GtkBuilder *builder = gtk_builder_new();
    gtk_builder_add_from_file (builder, "chat_window.glade", NULL);

    GtkEntry *chat_message_entry = GTK_ENTRY(gtk_builder_get_object(builder, "chat_message_entry"));
    printf("%ld\n", gtk_entry_get_type());
    printf("text from entry: \n");
    printf("%s\n", gtk_entry_get_text(chat_message_entry));
    g_object_unref(builder);
}

void on_btn_clicked() {
    gtk_main_quit();
}
