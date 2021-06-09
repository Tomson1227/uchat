#include "libmx.h"

int mx_sqrt(int x)
{
    if(x < 0) {
        return 0;
    }
    
    float temp = 0;
    float sqrt = x / 2;

    while(sqrt != temp){

        temp = sqrt;
        sqrt = (x/temp + temp) / 2;
    }

    return ((int)sqrt == sqrt) ? sqrt : 0;
}
