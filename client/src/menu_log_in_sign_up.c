//compilation: clang `pkg-config --cflags gtk+-3.0` main.c -o main `pkg-config --libs gtk+-3.0` -rdynamic
//pkg-config --cflags --libs gtk+-3.0 
#include <gtk/gtk.h>

typedef struct 
{
    GtkEntry *username;
    GtkEntry *password;
    GtkWidget *g_lbl_hello;
    GtkWidget *g_lbl_count;
    GtkGrid *log_in;
    GtkGrid *sign_up;
    GtkStack *stack;
    GtkEntry *username_sign_up;
    GtkEntry *password_sign_up;
    GtkEntry *repeat_password_sign_up;
    GtkGrid *grid;
    GtkButton *btn_back;
} app_widgets;

void load_css(void);
void on_window_main_destroy(void);
void on_btn_log_in_clicked(GtkButton *btn_log_in, app_widgets *wdgts);
void on_username_activate(GtkEntry *username, app_widgets *wdgts);
void on_password_activate(GtkEntry *password, app_widgets *wdgts);
void on_btn_sign_up_clicked(GtkButton *btn_sign_up, GtkStack *stack, app_widgets *wdgts);
void on_btn_sign_up_sign_up_clicked(GtkButton *btn_sign_up_sign_up, app_widgets *wdgts);

int main(int argc, char *argv[])
{
    GtkBuilder      *builder; 
    GtkWidget       *window;
    
    app_widgets        *widgets = g_slice_new(app_widgets);//allocating memory for structure with widgets
    gtk_init(&argc, &argv);
    load_css();
    builder = gtk_builder_new();
    gtk_builder_add_from_file (builder, "client/glades/window_main.glade", NULL);

    window = GTK_WIDGET(gtk_builder_get_object(builder, "window_main"));
    gtk_builder_connect_signals(builder, widgets);

    widgets->log_in = GTK_GRID(gtk_builder_get_object(builder, "log_in"));
    widgets->sign_up = GTK_GRID(gtk_builder_get_object(builder, "sign_up"));
    widgets->g_lbl_hello = GTK_WIDGET(gtk_builder_get_object(builder, "lbl_hello"));
    widgets->g_lbl_count = GTK_WIDGET(gtk_builder_get_object(builder, "lbl_count"));
    widgets->username = GTK_ENTRY(gtk_builder_get_object(builder, "username"));
    widgets->password = GTK_ENTRY(gtk_builder_get_object(builder, "password"));
    widgets->password_sign_up = GTK_ENTRY(gtk_builder_get_object(builder, "password_sign_up"));
    widgets->repeat_password_sign_up = GTK_ENTRY(gtk_builder_get_object(builder, "repeat_password_sign_up"));
    widgets->username_sign_up = GTK_ENTRY(gtk_builder_get_object(builder, "username_sign_up"));
    // gtk_entry_set_text(GTK_ENTRY(widgets->username), "");
    // gtk_entry_set_text(GTK_ENTRY(widgets->password), "");

    g_object_unref(builder);

    gtk_widget_show_all(window);                
    gtk_main();
    g_slice_free(app_widgets, widgets);//freeing memory allocated for widgets
    return 0;
}

void load_css()
{
    GtkCssProvider *provider;
    GdkDisplay *display;
    GdkScreen *screen;

    provider = gtk_css_provider_new();
    display = gdk_display_get_default();
    screen = gdk_display_get_default_screen(display);

    const gchar *css_style_file = "client/styles/style.css";
    GFile *css_fp = g_file_new_for_path(css_style_file);
    GError *error = 0;
    gtk_style_context_add_provider_for_screen(screen, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    gtk_css_provider_load_from_file(provider, css_fp, &error);
    g_object_unref(provider);
}

// called when window is closed
void on_window_main_destroy()
{
    gtk_main_quit();
}

void on_btn_log_in_clicked(GtkButton *btn_log_in, app_widgets *wdgts)
{
    char buffer_for_username[128];
    char buffer_for_password[128];
    sprintf(buffer_for_username, "%s", gtk_entry_get_text(wdgts->username));
    sprintf(buffer_for_password, "%s", gtk_entry_get_text(wdgts->password));
    printf("you entered username: %s\n", (const gchar*) buffer_for_username);
    printf("you entered password: %s\n", (const gchar*) buffer_for_password);
    //send_rq_log_in(char *username, char *password);
}

void on_username_activate(GtkEntry *username, app_widgets *wdgts)
{
    char buffer_for_username[128];
    char buffer_for_password[128];
    sprintf(buffer_for_username, "%s", gtk_entry_get_text(wdgts->username));
    sprintf(buffer_for_password, "%s", gtk_entry_get_text(wdgts->password));
    printf("you entered username: %s\n", (const gchar*) buffer_for_username);
    printf("you entered password: %s\n", (const gchar*) buffer_for_password);
}

void on_password_activate(GtkEntry *password, app_widgets *wdgts)
{
    char buffer_for_username[128];
    char buffer_for_password[128];
    sprintf(buffer_for_username, "%s", gtk_entry_get_text(wdgts->username));
    sprintf(buffer_for_password, "%s", gtk_entry_get_text(wdgts->password));
    printf("you entered username: %s\n", (const gchar*) buffer_for_username);
    printf("you entered password: %s\n", (const gchar*) buffer_for_password);
}

void on_btn_sign_up_clicked(GtkButton *btn_sign_up, GtkStack *stack, app_widgets *wdgts)
{
    gtk_entry_set_text(GTK_ENTRY(wdgts->username), "");

    g_return_if_fail(GTK_IS_BUTTON(btn_sign_up));
    g_return_if_fail(GTK_IS_STACK(stack));
    
    gtk_stack_set_visible_child_full ( stack, "sign_up", 0);
    g_print ( "Switching to %s.\n", gtk_stack_get_visible_child_name (stack));
    //clear log in entries
    
}

void on_btn_sign_up_sign_up_clicked(GtkButton *btn_sign_up_sign_up, app_widgets *wdgts)
{
    char buffer_for_username[128];
    char buffer_for_password[128];
    char buffer_for_password_repated[128];
    sprintf(buffer_for_username, "%s", gtk_entry_get_text(wdgts->username_sign_up));
    sprintf(buffer_for_password, "%s", gtk_entry_get_text(wdgts->password_sign_up));
    sprintf(buffer_for_password_repated, "%s", gtk_entry_get_text(wdgts->repeat_password_sign_up));
    printf("you entered username: %s\n", (const gchar*) buffer_for_username);
    printf("you entered password: %s\n", (const gchar*) buffer_for_password);
    printf("you repeated password: %s\n", (const gchar*) buffer_for_password_repated);
}

void on_btn_back_clicked(GtkButton *btn_back, GtkStack *stack, app_widgets *wdgts)
{
    g_return_if_fail(GTK_IS_BUTTON(btn_back));
    g_return_if_fail(GTK_IS_STACK(stack));
    
    gtk_stack_set_visible_child_full ( stack, "log_in", 0);
    g_print ( "Switching to %s.\n", gtk_stack_get_visible_child_name (stack));
}
