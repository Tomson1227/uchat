#include "cJSON.h"
#include <stdio.h>

/*SIGNUP
    From client to server request {
    "type": "REQUEST_SIGNUP",
    "login": "user_login",
    "pass": "user_pass",
}*/

char *send_rq_sign_in_client(char *username, char *password)
{
    char *string;
    cJSON *type = NULL;
    cJSON *login = NULL;
    cJSON *pass = NULL;
    cJSON *request_sign_up = cJSON_CreateObject();

    if (request_sign_up == NULL)
        cJSON_Delete(request_sign_up);
    else {
        type = cJSON_CreateString("REQUEST_SIGNUP");
        cJSON_AddItemToObject(request_sign_up, "type", type);
        login = cJSON_CreateString(username);
        pass = cJSON_CreateString(password);
        cJSON_AddItemToObject(request_sign_up, "type", type);
        cJSON_AddItemToObject(request_sign_up, "login", login);
        cJSON_AddItemToObject(request_sign_up, "pass", pass); 
        string = cJSON_Print(request_sign_up);
    }
    //send(socket, string , strlen(string) , IP );
    return string;
}
