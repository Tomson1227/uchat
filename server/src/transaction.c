#include "uchat_server.h"

#define BUFF_SIZE 1024

char *read_socket(int fd)
{
    char buff[BUFF_SIZE];
    char *line = NULL;
    ssize_t read_length;
    uint64_t message_length = 0;

    if((read_length = recv(fd, buff, BUFF_SIZE, 0)) < 0) {
        if(errno == ENOTCONN)
            return NULL;

        perror("recv fail");
        return NULL;
    }
    else if(read_length == 0)
        return NULL;

    if(!(line = (char *) calloc(read_length + 1, sizeof(char))))
        perror("allocation fail");
    
    strcpy(line, buff);
    
    while(read_length == BUFF_SIZE) {
        message_length += read_length;
        
        if((read_length = recv(fd, buff, BUFF_SIZE, 0)) < 0) {
            return NULL;
        }

        if(!(realloc(line, message_length + read_length + 1)))
            perror("allocation fail");
        
        strcpy(&line[message_length], buff);
    }

    return line;
}

int process_request(char *request, int fd)
{
    define_rq_type(request);

    free(request);

    return 0;
}
