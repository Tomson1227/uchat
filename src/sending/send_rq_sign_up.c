//#include "../libraries/cJSON/cJSON.h"
#include <stdio.h>

/*SIGNUP
    From client to server request {
    "type": REQUEST_SIGNUP,
    "login": "user_login",
    "pass": "user_pass",
}*/

char *send_rq_sign_up(char *username, char *password)
{
    char *string;
    cJSON *type = NULL;
    cJSON *login = NULL;
    cJSON *pass = NULL;
    cJSON *request_sign_up = cJSON_CreateObject();
    if (request_sign_up == NULL)
    {
        goto end;
    }
    type = cJSON_CreateString("REQUEST_SIGNUP");
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
    cJSON_AddItemToObject(request_sign_up, "type", type);
    cJSON_AddItemToObject(request_sign_up, "login", login);
    cJSON_AddItemToObject(request_sign_up, "pass", pass); 
    string = cJSON_Print(request_sign_up);

    //send(socket, string , strlen(string) , IP );

    end: 
        cJSON_Delete(request_sign_up);
        return string;
}

// int main() 
// {
//     char *username = "user";
//     char *password = "password";
//     printf("%s\n", send_rq_log_in(username, password));
// }
