#include "libmx.h"

int mx_strncmp(const char *src, const char *str, size_t n)
{
    long long count = 0;

    for(; count < (long long)(n - 1) && !(src[count] - str[count]); ++count);

    return (src[count] - str[count]);
}
