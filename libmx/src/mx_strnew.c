#include "libmx.h"

char *mx_strnew(const int size)
{
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
