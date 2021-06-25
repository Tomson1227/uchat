#include "libmx.h"

int mx_strncmp(const char *src, const char *str, size_t n)
{
    size_t count = 0;

    for(; count < (n - 1) && !(src[count] - str[count]); ++count);

    return (src[count] - str[count]);
}
