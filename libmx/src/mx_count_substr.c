#include "libmx.h"

int mx_count_substr(const char *str, const char *sub)
{
    if(str == NULL || sub == NULL) {
        return -1;
    }
    else if(sub[0] == '\0') {
        return 0;
    }

    int count = 0;
    char *pos = (char *)str;

    while(pos != NULL) {
        pos = mx_strstr(pos, sub); 
        count += (pos != NULL) ? 1 : 0;
        pos += (pos != NULL) ? 1 : 0;
    }

    return count;
}
