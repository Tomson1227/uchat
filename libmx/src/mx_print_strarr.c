#include "libmx.h"

void mx_print_strarr(char **arr, const char *delim)
{
    if(arr == NULL || delim == NULL)
        return;

    for(int i = 0; arr[i] != NULL;) {
        for(int j = 0; arr[i][j] != '\0'; ++j) {
            write(1, &arr[i][j], 1);
        }

        if(arr[++i] != NULL) {
            for(int j = 0; delim[j] != '\0'; ++j) {
                write(1, &delim[j], 1);
            }
        }
    }
    
    write(1, &"\n", 1);
}
