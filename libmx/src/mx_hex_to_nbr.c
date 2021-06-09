#include "libmx.h"

unsigned long mx_hex_to_nbr(const char *hex)
{
    if(hex == NULL)
        return 0;
    
    unsigned long number = 0;
        
    for(int i = 0; hex[i] != '\0'; ++i) {
        if(mx_isdigit(hex[i])) {
            number = number * 16 + (hex[i] - '0');
        }
        else {
            number = number * 16 + (10 + mx_tolower(hex[i]) - 'a');
        }
    }
    
    return number;
}
