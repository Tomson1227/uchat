#include "libmx.h"

int mx_get_substr_index(const char *str, const char *sub)
{
    if(str == NULL || sub == NULL) {
        return -2;
    }

    char *temp;

    if((temp = mx_strstr(str, sub)) != NULL) {
        return (temp - str);
    }

    return -1;
}
