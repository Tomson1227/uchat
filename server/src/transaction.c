#include "transaction.h"

char *server_responce(t_server_responce *response)
{
    char *line;
    size_t line_length = snprintf(NULL, 0, "%u;%u\n", response->response, 
                                  response->status);

    if(!(line = (char *) calloc(line_length, sizeof(char))))
        perror("allocation fail");

    sprintf(line, "%u;%u\n",response->response, response->status);
    return line;
}

int read_status(char *line)
{
    char *str = NULL;
    sscanf(line, "%*[^;];%[^;]", str);
    return atoi(str);
}

void *read_response(char *line) 
{
    char char_api[10];  
    sscanf(line, "%3[^;];", char_api);
    t_api api = atoi(char_api);
    t_server_responce *response;

    if(!(response = (t_server_responce *) calloc(1, sizeof(t_server_responce))))
        perror("allocation error");

    switch(api) {
        case REQUEST_LOGIN:
        case REQUEST_SIGNUP:
            response->response = api;
            sscanf(line, "%*[^;];%u", &response->status);
            return response;
            break;
        default:
            break;
    }
}

char *login_convert(t_user_request *request)
{
    char *line;
    size_t line_length = snprintf(NULL, 0, "%u;%s;%s\n", request->request, 
                                  request->user_logit, request->password);

    if(!(line = (char *) calloc(line_length, sizeof(char))))
        perror("allocation fail");

    sprintf(line, "%u;%s;%s\n", request->request, request->user_logit, 
            request->password);

    return line;
}

//@ pointer - point to created structure
//@ type - type of pass structure (see t_struct_type)
//# return pointer to corespond server structure or NULL if fail
void *send_request(void *pointer, t_struct_type type, int *sockfd)
{
    char *line = NULL; 

    switch(type) {
        case login_request:
        case signup_request:
            line = login_convert((t_user_request *) pointer);
            break;
        default:
            break;
    }

    // connection(sockfd);

    if(send(*sockfd, line, strlen(line), 0) < 0) {
        perror("tansmit fail");
        return NULL;
    }

    // return read_response();
    return NULL;
}

void connection(int *sockfd)
{
    if(!getpeername(*sockfd, NULL, NULL))
        ;
        // reconect();
}
