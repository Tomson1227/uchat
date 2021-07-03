#include "cJSON.h"
#include <stdio.h>

/*LOGIN
    From client to server request {
    "type": REQUEST_LOGIN,
    "login": "user_login",
    "pass": "user_pass",
}*/

char *send_rq_log_in(char *username, char *password)
{
    char *string;
    cJSON *type = NULL;
    cJSON *login = NULL;
    cJSON *pass = NULL;
    cJSON *request_log_in = cJSON_CreateObject();
    if (request_log_in == NULL)
    {
        goto end;
    }
    type = cJSON_CreateString("REQUEST_LOGIN");
    if (type == NULL)
    {
        goto end;
    }
    login = cJSON_CreateString(username);
    if (login == NULL)
    {
        goto end;
    }
    pass = cJSON_CreateString(password);
    if (pass == NULL)
    {
        goto end;
    }
    cJSON_AddItemToObject(request_log_in, "type", type);
    cJSON_AddItemToObject(request_log_in, "login", login);
    cJSON_AddItemToObject(request_log_in, "pass", pass); 
    string = cJSON_Print(request_log_in);

    //send(socket, string , strlen(string) , IP);

    end: 
        cJSON_Delete(request_log_in);
        return string;
}

// int main() 
// {
//     char *username = "user";
//     char *password = "password";
    
//     printf("%s\n", send_rq_log_in(username, password));
// }
