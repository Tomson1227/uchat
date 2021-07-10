#include "cJSON.h"
#include <stdio.h>

/*LOGIN
    request from client to server{
    "type": RESPONSE_LOGIN,
    "login": "user_login",
    "pass": "user_pass"
}*/

void receive_rq_log_in_server(const char * const string)
{
    //receiving login request from client
    // recv(sock, buffer, buffer_size, IP);

    cJSON *type = NULL;
    cJSON *login = NULL;
    cJSON *pass = NULL;
    cJSON *rq_log_in = cJSON_Parse(string);
    char *type_parsed = NULL;
    char *login_parsed = NULL;
    char *pass_parsed = NULL;
    int check;

    if (rq_log_in == NULL)
    {
        const char *error_ptr = cJSON_GetErrorPtr();
        if (error_ptr != NULL)
            fprintf(stderr, "Error before: %s\n", error_ptr);
        check = 0;
        cJSON_Delete(rq_log_in);
    }
    else  {
        type = cJSON_GetObjectItemCaseSensitive(rq_log_in, "type");
        login = cJSON_GetObjectItemCaseSensitive(rq_log_in, "login");
        pass = cJSON_GetObjectItemCaseSensitive(rq_log_in, "pass");
        type_parsed = cJSON_Print(type);
        login_parsed = cJSON_Print(login);
        pass_parsed = cJSON_Print(pass);
        printf("Received type: %s\nReceived login: %s\nReceived password: %s\n", type_parsed, login_parsed, pass_parsed);
    } 
}
