//compilation: clang `pkg-config --cflags gtk+-3.0` main.c -o main `pkg-config --libs gtk+-3.0` -rdynamic
#include <gtk/gtk.h>

typedef struct 
{
    /*entries*/
    GtkEntry *username;
    GtkEntry *password;
    GtkEntry *username_sign_up;
    GtkEntry *password_sign_up;
    GtkEntry *repeat_password_sign_up;
    /*labels*/
    GtkWidget *g_lbl_hello;
    GtkWidget *g_lbl_count;
    GtkWidget *g_lbl_error_display;
    GtkWidget *g_lbl_pill;
    GtkWidget *g_lbl_error_log_in;
    /*grids*/
    GtkGrid *log_in;
    GtkGrid *sign_up;
    GtkGrid *menu;
    GtkGrid *console;
    /*boxes*/
    /*stacks*/
    GtkStack *stack;
    /*buttons*/
    GtkButton *btn_back;
    GtkButton *btn_console;
    GtkButton *btn_gui;
    GtkButton *btn_log_in;
    GtkButton *btn_sign_up;
    GtkButton *btn_sign_up_sign_up;
    GtkButton *btn_back_log_in;
    GtkButton *btn_back_console;
    /*images*/
    //GtkImage *img_matrix;
} app_widgets;

void on_btn_back_console_clicked(GtkButton *btn_back_console, GtkStack *stack);
void on_btn_back_log_in_clicked(GtkButton *btn_back_log_in, GtkStack *stack);
void on_btn_console_clicked(GtkButton *btn_console_clicked, GtkStack *stack);
void on_btn_gui_clicked(GtkButton *btn_gui_clicked, GtkStack *stack);
void load_css_style(char *path);
void on_window_main_destroy(void);
void on_btn_log_in_clicked(GtkButton *btn_log_in, app_widgets *wdgts);
void on_username_activate(GtkEntry *username, app_widgets *wdgts);
void on_password_activate(GtkEntry *password, app_widgets *wdgts);
void on_btn_sign_up_clicked(GtkButton *btn_sign_up, GtkStack *stack, app_widgets *wdgts);
void on_btn_sign_up_sign_up_clicked(GtkButton *btn_sign_up_sign_up, app_widgets *wdgts);
void on_btn_back_clicked(GtkButton *btn_back, GtkStack *stack, app_widgets *wdgts);

int login_window(int argc, char *argv[])
{
    GtkBuilder      *builder; 
    GtkWidget       *window_main;
    
    app_widgets        *widgets = g_slice_new(app_widgets);//allocating memory for structure with widgets
    gtk_init(&argc, &argv);
    load_css_style("client/styles/main_menu.css");
    builder = gtk_builder_new();
    gtk_builder_add_from_file (builder, "client/glades/window_main.glade", NULL);

    window_main = GTK_WIDGET(gtk_builder_get_object(builder, "window_main"));
    gtk_builder_connect_signals(builder, widgets);
    
    /*grids*/
    widgets->log_in = GTK_GRID(gtk_builder_get_object(builder, "log_in"));
    widgets->sign_up = GTK_GRID(gtk_builder_get_object(builder, "sign_up"));
    widgets->menu = GTK_GRID(gtk_builder_get_object(builder, "menu"));
    widgets->console = GTK_GRID(gtk_builder_get_object(builder, "console"));
    /*labels*/
    widgets->g_lbl_hello = GTK_WIDGET(gtk_builder_get_object(builder, "lbl_hello"));
    widgets->g_lbl_count = GTK_WIDGET(gtk_builder_get_object(builder, "lbl_count"));
    widgets->g_lbl_error_display = GTK_WIDGET(gtk_builder_get_object(builder, "lbl_error_display"));
    widgets->g_lbl_pill = GTK_WIDGET(gtk_builder_get_object(builder, "lbl_pill"));
    widgets->g_lbl_error_log_in = GTK_WIDGET(gtk_builder_get_object(builder, "lbl_error_log_in"));
    /*entries*/
    widgets->username = GTK_ENTRY(gtk_builder_get_object(builder, "username"));
    widgets->password = GTK_ENTRY(gtk_builder_get_object(builder, "password"));
    widgets->password_sign_up = GTK_ENTRY(gtk_builder_get_object(builder, "password_sign_up"));
    widgets->repeat_password_sign_up = GTK_ENTRY(gtk_builder_get_object(builder, "repeat_password_sign_up"));
    widgets->username_sign_up = GTK_ENTRY(gtk_builder_get_object(builder, "username_sign_up"));
    /*buttons*/
    widgets->btn_back = GTK_BUTTON(gtk_builder_get_object(builder, "btn_back"));
    widgets->btn_log_in = GTK_BUTTON(gtk_builder_get_object(builder, "btn_log_in"));
    widgets->btn_sign_up = GTK_BUTTON(gtk_builder_get_object(builder, "btn_sign_up"));
    widgets->btn_sign_up_sign_up = GTK_BUTTON(gtk_builder_get_object(builder, "btn_sign_up_sign_up"));
    widgets->btn_console = GTK_BUTTON(gtk_builder_get_object(builder, "btn_console"));
    widgets->btn_gui = GTK_BUTTON(gtk_builder_get_object(builder, "btn_gui"));
    widgets->btn_back_log_in = GTK_BUTTON(gtk_builder_get_object(builder, "btn_back_log_in"));
    widgets->btn_back_console = GTK_BUTTON(gtk_builder_get_object(builder, "btn_back_console"));

    g_object_unref(builder);
    gtk_widget_show_all(window_main);                
    gtk_main();
    g_slice_free(app_widgets, widgets);//freeing memory allocated for widgets
    return 0;
}

void on_btn_back_console_clicked(GtkButton *btn_back_console, GtkStack *stack)
{
    gtk_stack_set_visible_child_full (stack, "menu", 0);
    load_css_style("client/styles/main_menu.css");
}

void on_btn_back_log_in_clicked(GtkButton *btn_back_log_in, GtkStack *stack)
{
    gtk_stack_set_visible_child_full (stack, "menu", 0);
    load_css_style("client/styles/main_menu.css");
}

void on_btn_console_clicked(GtkButton *btn_console_clicked, GtkStack *stack)
{   
    gtk_stack_set_visible_child_full (stack, "console", 0);
    load_css_style("client/styles/console.css");
}

void on_btn_gui_clicked(GtkButton *btn_gui_clicked, GtkStack *stack)
{
    gtk_stack_set_visible_child_full (stack, "log_in", 0);
    load_css_style("client/styles/login_signup.css");
    g_print("Switching to %s.\n", gtk_stack_get_visible_child_name (stack));
}

void load_css_style(char *path)
{
    GtkCssProvider *provider;
    GdkDisplay *display;
    GdkScreen *screen;

    provider = gtk_css_provider_new();
    display = gdk_display_get_default();
    screen = gdk_display_get_default_screen(display);

    const gchar *css_style_file = path;
    GFile *css_fp = g_file_new_for_path(css_style_file);
    GError *error = 0;
    gtk_style_context_add_provider_for_screen(screen, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    gtk_css_provider_load_from_file(provider, css_fp, &error);
    g_object_unref(provider);
}

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
    if (strlen(buffer_for_username) > 0 && strlen(buffer_for_username) > 0)
        gtk_label_set_text(GTK_LABEL(wdgts->g_lbl_error_log_in), "");
    else if (strlen(buffer_for_password) == 0 || strlen(buffer_for_username) == 0 || (strlen(buffer_for_password) == 0 && strlen(buffer_for_username) == 0))
        gtk_label_set_text(GTK_LABEL(wdgts->g_lbl_error_log_in), "please enter your credentials");
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
    if (strlen(buffer_for_username) > 0 && strlen(buffer_for_username) > 0)
        gtk_label_set_text(GTK_LABEL(wdgts->g_lbl_error_log_in), "");
    else if (strlen(buffer_for_password) == 0 || strlen(buffer_for_username) == 0 || (strlen(buffer_for_password) == 0 && strlen(buffer_for_username) == 0))
        gtk_label_set_text(GTK_LABEL(wdgts->g_lbl_error_log_in), "please enter your credentials");
    printf("you entered username: %s\n", (const gchar*) buffer_for_username);
    printf("you entered password: %s\n", (const gchar*) buffer_for_password);
}

void on_password_activate(GtkEntry *password, app_widgets *wdgts)
{
    char buffer_for_username[128];
    char buffer_for_password[128];

    sprintf(buffer_for_username, "%s", gtk_entry_get_text(wdgts->username));
    sprintf(buffer_for_password, "%s", gtk_entry_get_text(wdgts->password));
    if (strlen(buffer_for_username) > 0 && strlen(buffer_for_username) > 0)
        gtk_label_set_text(GTK_LABEL(wdgts->g_lbl_error_log_in), "");
    else if (strlen(buffer_for_password) == 0 || strlen(buffer_for_username) == 0 || (strlen(buffer_for_password) == 0 && strlen(buffer_for_username) == 0))
        gtk_label_set_text(GTK_LABEL(wdgts->g_lbl_error_log_in), "please enter your credentials");
    printf("you entered username: %s\n", (const gchar*) buffer_for_username);
    printf("you entered password: %s\n", (const gchar*) buffer_for_password);
}

void on_btn_sign_up_clicked(GtkButton *btn_sign_up, GtkStack *stack, app_widgets *wdgts)
{
    g_return_if_fail(GTK_IS_BUTTON(btn_sign_up));
    g_return_if_fail(GTK_IS_STACK(stack));
    gtk_stack_set_visible_child_full (stack, "sign_up", 0);
    g_print ( "Switching to %s.\n", gtk_stack_get_visible_child_name (stack));
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
    if (strlen(buffer_for_username) == 0 || strlen(buffer_for_password) == 0 || strlen(buffer_for_password_repated) == 0)
        gtk_label_set_text(GTK_LABEL(wdgts->g_lbl_error_display), "please enter your credentials");
    else if (strcmp(buffer_for_password, buffer_for_password_repated) != 0) 
        gtk_label_set_text(GTK_LABEL(wdgts->g_lbl_error_display), "passwords do not match");
    else if (strcmp(buffer_for_password, buffer_for_password_repated) == 0 && strlen(buffer_for_password) < 6)
        gtk_label_set_text(GTK_LABEL(wdgts->g_lbl_error_display), "password should inlcude\n   more than 6 symbols");
    else 
    {
        gtk_label_set_text(GTK_LABEL(wdgts->g_lbl_error_display), "");
        load_css_style("client/styles/signup.css");
        //gtk_stack_set_visible_child_full (stack, "box_img", 0);
        //g_print("Switching to %s.\n", gtk_stack_get_visible_child_name (stack));
    }
}

void on_btn_back_clicked(GtkButton *btn_back, GtkStack *stack, app_widgets *wdgts)
{
    g_return_if_fail(GTK_IS_BUTTON(btn_back));
    g_return_if_fail(GTK_IS_STACK(stack));
    gtk_stack_set_visible_child_full (stack, "log_in", 0);
    load_css_style("client/styles/login_signup.css");
    g_print("Switching to %s.\n", gtk_stack_get_visible_child_name (stack));
}