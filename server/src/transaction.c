#include "transaction.h"

#define BUFF_SIZE 1024

int server_responce(t_server_responce *response, int fd)
{
    int status;
    char *line;
    size_t line_length = snprintf(NULL, 0, "%u;%u\n", response->response, 
                                  response->status);

    if(!(line = (char *) calloc(line_length, sizeof(char))))
        perror("allocation fail");

    sprintf(line, "%u;%u\n", response->response, response->status);
    status = send(fd, line, line_length, 0);
    free(line);
    
    return status < 0;
}

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

// int read_status(char *line)
// {
//     char *str = NULL;
//     sscanf(line, "%*[^;];%[^;]", str);
//     return atoi(str);
// }

// void *read_response(char *line) 
// {
//     char char_api[10];  
//     sscanf(line, "%3[^;];", char_api);
//     t_api api = atoi(char_api);
//     t_server_responce *response;

//     if(!(response = (t_server_responce *) calloc(1, sizeof(t_server_responce))))
//         perror("allocation error");

//     switch(api) {
//         case REQUEST_LOGIN:
//         case REQUEST_SIGNUP:
//             response->response = api;
//             sscanf(line, "%*[^;];%u", &response->status);
//             return response;
//             break;
//         default:
//             break;
//     }
// }

// char *login_convert(t_user_request *request)
// {
//     char *line;
//     size_t line_length = snprintf(NULL, 0, "%u;%s;%s\n", request->request, 
//                                   request->user_logit, request->password);

//     if(!(line = (char *) calloc(line_length, sizeof(char))))
//         perror("allocation fail");

//     sprintf(line, "%u;%s;%s\n", request->request, request->user_logit, 
//             request->password);

//     return line;
// }

// //@ pointer - point to created structure
// //@ type - type of pass structure (see t_struct_type)
// //# return pointer to corespond server structure or NULL if fail
// void *send_request(void *pointer, t_struct_type type, int *sockfd)
// {
//     char *line = NULL; 

//     switch(type) {
//         case login_request:
//         case signup_request:
//             line = login_convert((t_user_request *) pointer);
//             break;
//         default:
//             break;
//     }

//     // connection(sockfd);

//     if(send(*sockfd, line, strlen(line), 0) < 0) {
//         perror("tansmit fail");
//         return NULL;
//     }

//     // return read_response();
//     return NULL;
// }

// void connection(int *sockfd)
// {
//     if(!getpeername(*sockfd, NULL, NULL))
//         ;
//         // reconect();
// }

int read_api(char *line)
{
    char *str = NULL;
    sscanf(line, "%[^;]", str);

    return atoi(str);
}

// int send_respond(void *respond, t_struct_type type, int fd)
// {
//     int status;
//     char *massege = NULL;

//     switch(type) {
//         case login_request:
//             break;
//         case signup_request:
//             break;
//         default:
//             break;
//     }

//     status = send(fd, massege, strlen(massege), 0);
//     return status < 0;
// }

int process_request(char *request, int fd)
{
    define_rq_type(request);

    free(request);
    // printf("%s\n", request);
    // t_server_responce response;
    // response.response = REQUEST_LOGIN;
    // response.status = LOGIN_OK;            
    // server_responce(&response, fd);            
    return 0;
}
