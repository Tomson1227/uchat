#include "libmx.h"

int mx_count_chars(char *str, char src)
{
    int count = 0;

    for(int index = 0; str[index]; ++index) {
        if(str[index] == src)
            ++count;
    }

    return count;
}
