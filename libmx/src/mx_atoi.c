#include "libmx.h"

int mx_atoi(const char *str) {
    int count = 0;
    int sum = 0;
    int flag = 1;
    
    while(mx_isspace(str[count++]));
    
    if(str[count] == '-' || str[count] == '+')
        flag = (str[count++] == '-') ? -1 : 1;

    for(;str[count] == '-' || str[count] == '+'; ++count);

    while (str[count]) {
        if (!mx_isdigit(str[count]))
            break;
        sum = sum * 10 + str[count++] - '0';
    }
    
    return sum * flag;
}
