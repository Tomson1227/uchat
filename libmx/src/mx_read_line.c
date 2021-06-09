#include "libmx.h"

int mx_read_line(char **lineptr, size_t buf_size, char delim, const int fd) {
    if (buf_size < 0 || fd < 0) {
        return -2;
    }

    
    (*lineptr) = (char *) mx_realloc(*lineptr, buf_size);
    size_t lineptr_size = malloc_size((*lineptr));
    mx_memset((*lineptr), '\0', lineptr_size);
    size_t bytes = 0;
    char buf;

    if (read(fd, &buf, 1)) {
        if (buf == delim) {
            return bytes;
        }

        if(bytes > lineptr_size) {
            (*lineptr) = (char *) mx_realloc(*lineptr, bytes + 1);
            lineptr_size = malloc_size((*lineptr));
            
        }
        (*lineptr)[bytes++] = buf;
    }
    else {
        return -1;
    }

    while (read(fd, &buf, 1)) {
        if (buf == delim) {
            break;
        }
        
        if (bytes >= lineptr_size) {
            (*lineptr) = (char *) mx_realloc(*lineptr, bytes + 1);
            lineptr_size = malloc_size((*lineptr));
        }

        (*lineptr)[bytes++] = buf;
    }

    if(bytes + 1 > lineptr_size) {
        (*lineptr) = (char *) mx_realloc(*lineptr, bytes + 1);
        lineptr_size = malloc_size((*lineptr));
    }
    size_t free_bytes = lineptr_size - bytes;
    mx_memset(&(*lineptr)[bytes++], '\0', free_bytes);

    return bytes;
}
