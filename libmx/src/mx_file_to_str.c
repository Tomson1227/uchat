#include "libmx.h"

char *mx_file_to_str(const char *file)
{
    int fd, i = 0, ch, len = 0; 
    char *output;
	
	fd = open(file, O_RDONLY);
    if (fd > 0) {
        while(read(fd,&ch,1))
            len++;
        close(fd);
        if (len > 0) {
            fd = open(file, O_RDONLY);
            output = mx_strnew(len);
            while(read(fd,&ch,1)) {
                output[i] = ch;
                i++;
            }
            close(fd);
            return output;
        }
    }                                   	
	return NULL;
}
