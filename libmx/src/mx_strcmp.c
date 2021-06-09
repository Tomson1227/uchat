#include "libmx.h"

int mx_strcmp(const char *s1, const char *s2)
{
    int count = 0;

    while(!(s1[count] - s2[count]) && s1[count])
        count++;    

    return (s1[count] - s2[count]);
}
