#include "libmx.h"

void mx_print_list(char ***list, char *dilim1, char *dilim2)
{
    if(list == NULL || dilim1 == NULL || dilim2 == NULL)
        return;

    for(int i = 0; list[i] != NULL;) {
        int j = 0;
        mx_printstr(list[i][j++]);

        for(; list[i][j] != NULL; ++j) {
            mx_printstr(dilim1);
            mx_printstr(list[i][j]);
        }

        if(list[++i] != NULL)
            mx_printstr(dilim2);
    }

    mx_printchar('\n');
}
