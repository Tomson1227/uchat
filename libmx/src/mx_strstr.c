#include "libmx.h"

char *mx_strstr(const char *haystack, const char *needle)
{
    int needle_size = mx_strlen(needle);
    int haystack_size = mx_strlen(haystack);
    char *temp = (char *)haystack;
    
    if(!needle_size)
        return (char *)haystack;

    for(int i = 0; i < haystack_size; ++i) {
        
        if(!mx_strncmp(&temp[i], needle, needle_size)) {
            return &temp[i];
        }
    }

    return NULL;
}
