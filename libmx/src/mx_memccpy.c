#include "libmx.h"

void *mx_memccpy(void *restrict dst, const void *restrict src, int c, size_t n)
{
    size_t i = 0;

    for(; i < n; ++i) {
        ((unsigned char *) dst)[i] = ((unsigned char *) src)[i];
        if(((unsigned char *) dst)[i] == c) {
            return (void *) &(((unsigned char *)dst)[i + 1]);
        }
    }

    return NULL;
}
