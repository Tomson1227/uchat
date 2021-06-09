#include "libmx.h"

char *mx_path_build(char *str1, char *str2, char *str3)
{
    char *temp1 = mx_strjoin(str2, str3);
    char *temp2 = mx_strjoin(str1, temp1);

    free(temp1);
    return temp2;
}
