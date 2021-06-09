#include "libmx.h"

int mx_tolower(int c)
{
    return (c >= 'A' && c <= 'Z') ? c + 0x20 : c; 
}
