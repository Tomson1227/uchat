#include "uchat.h"

char *my_itoa(long long number) {
    char *str = (char *)malloc(21);
	int count = 0;
	unsigned long long num;
    int flag = 0;

	if(number & 0x8000000000000000) {
        flag = 1;
		str[count++] = '-';
		num = (number ^ 0xFFFFFFFFFFFFFFFF) + 1;
	}
	else
		num = (unsigned long long) number;
	if(num == 0)
		str[count++] = '0';
	for(; num != 0; num /= 10)
		str[count++] = ((num % 10) + '0');
    for(int i = 0; (i + flag) < (count + flag) / 2; ++i) {
            char temp = str[i + flag];
            str[i + flag] = str[count - i - 1];
            str[count - i - 1] = temp;
        }
    str[count] = '\0';
    return str;
}

char *mx_strnew(const int size) {
    if(size >= 0) {
        char *strnew = (char *) malloc(size + 1);
        
        if(NULL == strnew)
            return NULL;
        
        for(int i = 0; i <= size; ++i) {
            strnew[i] = '\0';
        }
        
        return strnew;
    }
    
    return NULL;
}

char *mx_strdup(const char *str) {
    char *strcopy = mx_strnew(strlen(str));
    return mx_strcpy(strcopy, str);
}

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

void mx_free(void **ptr) {
    if (ptr && *ptr) {
        free(*ptr);
        *ptr = NULL;
    }
}
