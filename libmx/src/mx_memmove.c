#include "libmx.h"

void *mx_memmove(void *dst, const void *src, size_t len)
{
    unsigned char *temp = (unsigned char *) malloc(len);
    unsigned char *source = (unsigned char *) src;
    unsigned char *dest =  (unsigned char *) dst;
    
    for(size_t i = 0; i < len; ++i) {
        temp[i] = source[i];
    }

    for(size_t i = 0; i < len; ++i) {
        dest[i] = temp[i];
    }

    free(temp);
    return dst;
}
