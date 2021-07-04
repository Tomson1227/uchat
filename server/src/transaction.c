#include "transaction.h"

char *login_convert(t_login_request *request)
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

// read_request(t_login_request *reqvese, char *line)
// {
//     sscanf(line, "%[^;]%u;%[^;]%s", requese->request, requese->user_logit, requese);
// }
