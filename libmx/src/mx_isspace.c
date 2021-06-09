#include "libmx.h"

bool mx_isspace(char c) 
{
    return (c == ' ' || (c >= 9 && c <= 13)) ? true : false;
}
