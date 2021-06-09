#include "libmx.h"

int mx_count_words(const char *str, char delimiter)
{
    if(str == NULL) {
        return -1;
    }

    int i = 0;
    int pos = i;
    int count = 0;

    while(str[i] != '\0') {  
        while(str[i] == delimiter && str[i] != '\0') {
            ++i;
        }

        pos = i;      
        
        while(str[i] != delimiter && str[i] != '\0') {
            ++i;
        }

        if(pos - i) {
            ++count;
        }
    }

    return count;
}
