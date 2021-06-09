#include "libmx.h"

char *mx_strdup(const char *str)
{
    char *strcopy = mx_strnew(mx_strlen(str));
    return mx_strcpy(strcopy, str);
}
