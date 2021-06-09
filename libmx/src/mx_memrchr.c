#include "libmx.h"

void *mx_memrchr(const void *s, int c, size_t n)
{   
    const unsigned char *p = (const unsigned char *)s + n;

    for (; n > 0; n--, p--) {
        if (*p == c) 
            return (void *)p;
    }

    return NULL;
}
