#include "libmx.h"

char *mx_strtrim(const char *str)
{
    if(str == NULL) {
        return NULL;
    }

    int left, right;

    for(left = 0; mx_isspace(str[left]); ++left);
    for(right = mx_strlen(str); mx_isspace(str[right - 1]); --right);

    char *dup = mx_strndup(&((char *)str)[left], (right - left));

    return dup;
}
