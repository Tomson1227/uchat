#include "uchat.h"

void set_widget_class(GtkWidget *widget, gchar *class_name) {
    GtkStyleContext *context = gtk_widget_get_style_context(widget);

    gtk_style_context_add_class(context, class_name);
}
