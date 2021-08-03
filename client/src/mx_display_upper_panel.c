#include "uchat.h"

void mx_display_upper_panel(GtkListBox *box, GtkListBoxRow *r, GtkStack *stack_upper_dialog_toolbar) {
    if (r) {
        if (gtk_list_box_row_get_selectable(r) == TRUE)
            gtk_stack_set_visible_child_name (stack_upper_dialog_toolbar, "chat_topbar_wth_btns");
        if (gtk_list_box_row_get_selectable(r) == FALSE)
            gtk_stack_set_visible_child_name(stack_upper_dialog_toolbar, "chat_topbar_grid");
    }
}
