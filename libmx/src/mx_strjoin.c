#include "libmx.h"

char *mx_strjoin(char const *s1, char const *s2)
{
    if(s1 == NULL || s2 == NULL) {
        if(s1 == NULL && s2 == NULL)
             return NULL;
        else if(s1 == NULL)
            return mx_strdup(s2);
        else
            return mx_strdup(s1);
    }

    char *new_str = mx_strnew(mx_strlen(s1) + mx_strlen(s2));
    
    if(!new_str)
        return NULL;

    new_str = mx_strcpy(new_str, s1);
    new_str = mx_strcat(new_str, s2);
    return new_str;
}
