#include "libmx.h"

int mx_list_size(t_list *list)
{
    t_list *copy = list;
    int count = 0;

    while (copy != NULL) {
        copy = copy->next;
        ++count;
    }

    return count;
}
