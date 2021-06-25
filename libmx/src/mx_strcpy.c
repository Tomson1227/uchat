#include "libmx.h"

char *mx_strcpy(char *dst, const char *src) {
    char *dst_return = dst;
    while (*src)
    {
        *dst = *src;
        dst++;
        src++;
    }
    *dst = '\0';
    return dst_return;
}
// #include "libmx.h"

// char *mx_strcpy(char *dst, const char *src)
// {
//     dst[0] = '\0'; 

//     for(size_t i = 0; src[i];) {
//         dst[i] = src[i];
//         dst[++i] = '\0'; 
//     }

//     return dst;
// }
