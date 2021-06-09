#include "libmx.h"

char *mx_nbr_to_hex(unsigned long nbr)
{
    int count = 0;
    unsigned long nbr_copy = nbr;
    
    for(;nbr_copy != 0; nbr_copy /= 16, ++count);

    int number;
    int i = 0; 
    char *new_hex = mx_strnew(count + 1);  
    
    if(new_hex == (void *)0 || nbr == 0)
        return (char *) &"0";
    
    for(; i < count; ++i, nbr /= 16) {
        if((number = nbr % 16) < 10)
            new_hex[count - i - 1] = number + '0';
        else
            new_hex[count - i - 1] = number + 'a' - 10;
    }
    
    new_hex[count] = '\0';
    
    return new_hex;
}
