#include "libmx.h"

void mx_push_back(t_list **list, void *data)
{
    if(list == NULL)
        return;

    t_list *new_node = mx_create_node(data);  

    if(*list == NULL) {
        *list = new_node;
    }
    else {
        t_list *last_node = *list;

        while (last_node -> next != NULL) {
            last_node = last_node -> next;
        }

        last_node -> next = new_node;
    }   
}
