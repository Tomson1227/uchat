#define __STDC_WANT_LIB_EXT1__ 1

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <locale.h>
#include <string.h>
#include <sys/poll.h>
#include <sys/socket.h> 
#include <sys/errno.h>

#ifdef __linux__
    #include <malloc.h> // For Linux
    #define malloc_size malloc_usable_size
#else
    #include <malloc/malloc.h> //For macOS
#endif

char *read_socket(int fd);
void *read_response(char *line);
int process_request(char *request, int fd);
