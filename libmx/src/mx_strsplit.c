#include "libmx.h"

static int mx_count_letters(const char *str, char c)
{
    int i = 0;
    for(;str[i] != '\0' && str[i] != c; ++i);
    
    return i;
}

char **mx_strsplit(const char *s, char c)
{
    if (!s) {
        return NULL;
    }

    int word_length = 0;
    int i = 0;
    char **arr = NULL;
    arr = (char **)malloc((mx_count_words(s, c) + 1) * sizeof(char *));

    while ((*s) && (*s != '\0')) {
        if (*s != c) {
            word_length = mx_count_letters(s, c);
            arr[i] = mx_strndup(s, word_length);
            s += word_length;
            i++;
            continue;
        }
        s++;
    }

    arr[i] = NULL;
    
    return arr;
}
