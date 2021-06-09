#include "libmx.h"

void mx_printstr(const char *s)
{
     int count = 0;

     while(s[count]) {
          write(1, &s[count++], 1);
     }
}
