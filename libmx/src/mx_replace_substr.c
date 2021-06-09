#include "libmx.h"

char *mx_replace_substr(const char *str, const char* sub, const char *replace)
{
    if (!str || !sub || !replace) {
		return NULL;
    }

    int num_of_sub = mx_count_substr(str, sub);

    if(num_of_sub == -1) {
        return mx_strdup(str);
    }

	int replace_len = mx_strlen(replace);
    int sub_len = mx_strlen(sub);
	int len = mx_strlen(str) + num_of_sub * (replace_len - sub_len);
	char *memory = mx_strnew(len);
    char *temp = mx_strstr(str, sub);
    size_t i = 0;
    size_t j = 0;
    
    while(temp != NULL) {
        mx_strncpy(&memory[i], &str[j], temp - &str[j]);
        i += temp - &str[j];
        j = temp + sub_len - str;
        mx_strcpy(&memory[i], replace);
        i += replace_len;
        temp = mx_strstr(&str[j], sub);
    }

    mx_strcpy(&memory[i], &str[j]);

    return memory;
}
