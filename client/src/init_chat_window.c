#include "uchat.h"

// gint ndx;
// int i = 0;
// void AddListItem(t_chat *wdgts, char *sText);
// void *get_time(t_chat *wdgts);

// void AddListItem(t_chat *wdgts, char *sText) {
//     GtkListBox *box = get_current_room(wdgts);
//     load_css("client/styles/messenger_window.css");
//     wdgts->msg->row_msg = gtk_list_box_row_new();
//     gtk_list_box_insert(box, wdgts->msg->row_msg, -1); 
//     gtk_widget_show(wdgts->msg->row_msg);
//     wdgts->label_msg = gtk_label_new(sText);
//     gtk_container_add(GTK_CONTAINER(wdgts->msg->row_msg), wdgts->label_msg);
//     gtk_list_box_row_set_selectable((GtkListBoxRow *)wdgts->msg->row_msg, TRUE); 
//     gtk_widget_set_halign(wdgts->label_msg, GTK_ALIGN_END);
//     gtk_widget_show (wdgts->label_msg);
// }

// void *get_time(t_chat *wdgts) {
//     GDateTime   *time;            // for storing current time and date
//     gchar       *time_str;        // current time and date as a string
//     time     = g_date_time_new_now_local();             // get the current time
//     time_str = g_date_time_format(time, "%H:%M:%S");    // convert current time to string

//     GtkListBox *box = get_current_room(wdgts);
//     gtk_widget_show(GTK_WIDGET(box));
//     wdgts->msg->row_time = gtk_list_box_row_new();
//     gtk_list_box_row_set_selectable((GtkListBoxRow *)wdgts->msg->row_time, FALSE);    
//     gtk_list_box_insert (box, wdgts->msg->row_time, -1); 
//     wdgts->msg->msg_date = gtk_label_new(time_str);
//     gtk_label_set_selectable((GtkLabel *)wdgts->msg->msg_date, FALSE);
//     gtk_container_add(GTK_CONTAINER(wdgts->msg->row_time), wdgts->msg->msg_date);
//     gtk_widget_set_halign(wdgts->msg->msg_date, GTK_ALIGN_END);
//     gtk_widget_show (wdgts->msg->msg_date);
//     g_free(time_str);
//     g_date_time_unref(time);
//     gtk_widget_show (wdgts->msg->row_time);
// }

// void on_listbox_msgs_row_selected(GtkListBox *box, GtkListBoxRow *row, GtkStack *stack_upper_dialog_toolbar, t_chat *wdgts) {
//     // g_return_if_fail (GTK_IS_LIST_BOX (box));
//     // g_return_if_fail (row == NULL || GTK_IS_LIST_BOX_ROW (row));

//     // box = GTK_LIST_BOX(cpy->lst_bx);
//     // row = GTK_LIST_BOX_ROW(cpy->r);

//     // ndx = gtk_list_box_row_get_index(row);
//     // printf("%d\n", ndx);

//     // if (cpy->r && gtk_list_box_row_get_selectable(GTK_LIST_BOX_ROW(cpy->r)) == TRUE)
//     //     gtk_stack_set_visible_child_name (stack_upper_dialog_toolbar, "chat_topbar_wth_btns");
//     // if (cpy->r && gtk_list_box_row_get_selectable(GTK_LIST_BOX_ROW(cpy->r)) == FALSE)
//     //     gtk_stack_set_visible_child_name(stack_upper_dialog_toolbar, "chat_topbar_grid");
// }

// void on_btn_delete_msg_clicked(GtkButton *btn_delete_msg, t_chat *wdgts) {
//     // int indx = gtk_list_box_row_get_index(gtk_list_box_get_selected_row((GtkListBox *)cpy->lst_bx));
//     // gtk_container_remove((GtkContainer *)cpy->lst_bx, (GtkWidget *)gtk_list_box_get_row_at_index((GtkListBox *)cpy->lst_bx, indx));
//     // gtk_container_remove((GtkContainer *)cpy->lst_bx, (GtkWidget *)gtk_list_box_get_row_at_index((GtkListBox *)cpy->lst_bx, indx));
// }

// void create_room(GtkButton *btn, t_chat *wdgts) { 
//     //send request
//     //receive response
//     //fill_room(room);

//     wdgts->curr_chat->room_id = i;
//     printf("i: %d\nroom_id: %d\n",i, wdgts->curr_chat->room_id);

//     wdgts->curr_chat->row_chat = gtk_list_box_row_new();
//     gtk_list_box_insert(wdgts->listbox_dlgs, wdgts->curr_chat->row_chat, -1);
//     gtk_widget_show((GtkWidget *)wdgts->curr_chat->row_chat);
//     g_object_set_data(wdgts->curr_chat->row_chat, "id_room", wdgts->curr_chat->room_id);

//     wdgts->curr_chat->chat_name = gtk_label_new(my_itoa(i));
//     gtk_container_add(GTK_CONTAINER(wdgts->curr_chat->row_chat), (GtkWidget *)wdgts->curr_chat->chat_name);
//     gtk_widget_show((GtkWidget *)wdgts->curr_chat->chat_name);
    
//     wdgts->curr_chat->scrll_wndw_msgs = gtk_scrolled_window_new(NULL, NULL);
//     gtk_stack_add_named(wdgts->stack, GTK_WIDGET(wdgts->curr_chat->scrll_wndw_msgs), my_itoa(i));
//     gtk_widget_show((GtkWidget *)wdgts->curr_chat->scrll_wndw_msgs);

//     wdgts->curr_chat->vwprt = gtk_viewport_new(NULL, NULL);
//     gtk_container_add(GTK_CONTAINER(wdgts->curr_chat->scrll_wndw_msgs), GTK_WIDGET(wdgts->curr_chat->vwprt));
//     gtk_widget_show((GtkWidget *)wdgts->curr_chat->vwprt);

//     wdgts->curr_chat->listbox_msgs = gtk_list_box_new();
//     gtk_container_add(GTK_CONTAINER(wdgts->curr_chat->vwprt), GTK_WIDGET(wdgts->curr_chat->listbox_msgs));
//     gtk_widget_show((GtkWidget *)wdgts->curr_chat->listbox_msgs);
//     //g_signal_connect(wdgts->tmp->lst_bx, "row-selected", G_CALLBACK(mx_display_upper_panel), wdgts->stack_upper_dialog_toolbar);

//     gtk_stack_set_visible_child(wdgts->stack, (GtkWidget *)wdgts->curr_chat->scrll_wndw_msgs);
//     g_print("Switching to %s.\n", gtk_stack_get_visible_child_name (wdgts->stack));

//     wdgts->curr_chat->row_msg = gtk_list_box_row_new();
//     gtk_list_box_insert(GTK_LIST_BOX(wdgts->curr_chat->listbox_msgs), wdgts->curr_chat->row_msg, -1); 
//     gtk_widget_show (wdgts->curr_chat->row_msg);
    
//     wdgts->curr_chat->lbl = gtk_label_new(my_itoa(i));
//     gtk_container_add(GTK_CONTAINER(wdgts->curr_chat->row_msg), wdgts->curr_chat->lbl); 
//     gtk_widget_set_halign(wdgts->curr_chat->lbl, GTK_ALIGN_END);
//     gtk_widget_show (wdgts->curr_chat->lbl);
//     g_object_set_data(wdgts->curr_chat->row_chat, "room", wdgts->curr_chat->listbox_msgs);
//     wdgts->curr_chat->row_index = gtk_list_box_row_get_index(wdgts->curr_chat->row_chat);
//     i++;
// }

// void on_listbox_dlgs_row_selected(GtkListBox *box, GtkListBoxRow *row, t_chat *wdgts) {
//     wdgts->curr_chat->row_index = gtk_list_box_row_get_index(row); 
//     printf("index: %d\n", wdgts->curr_chat->row_index);

//     printf("switching to room id:%d\n", g_object_get_data(row, "id_room"));
//     const gchar *s = my_itoa((long long int)g_object_get_data(row, "id_room"));
//     gtk_stack_set_visible_child_name(wdgts->stack, s);
//     free(s);
// }

// GtkListBox *get_current_room(t_chat *wdgts) {
//     GtkListBox *box = NULL; 

//     if (wdgts->curr_chat->row_index == -1) {
//         printf("No row is selected\n");
//         box = wdgts->curr_chat->listbox_msgs;
//     }
//     else {
//         GtkListBoxRow *row = gtk_list_box_row_new();
//         printf("index check: %d\n", wdgts->curr_chat->row_index);
//         row = gtk_list_box_get_row_at_index(wdgts->listbox_dlgs, wdgts->curr_chat->row_index);
//         box = g_object_get_data(G_OBJECT(row), "room");
//         if (box == NULL)
//            printf("errorrrr\n"); 
//     }
//     return box;
// }