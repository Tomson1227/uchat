#include "cJSON.h"
#include <stdio.h>

/*LOGIN
    From client to server request {
    "type": REQUEST_LOGIN,
    "login": "user_login",
    "pass": "user_pass",
}*/

char *send_rq_log_in_client(char *username, char *password)
{
    char *string;
    cJSON *type = NULL;
    cJSON *login = NULL;
    cJSON *pass = NULL;
    cJSON *request_log_in = cJSON_CreateObject();

    if (request_log_in == NULL)
        cJSON_Delete(request_log_in);
    else {
        type = cJSON_CreateString("REQUEST_LOGIN");
        cJSON_AddItemToObject(request_log_in, "type", type);
        login = cJSON_CreateString(username);
        pass = cJSON_CreateString(password);
        cJSON_AddItemToObject(request_log_in, "type", type);
        cJSON_AddItemToObject(request_log_in, "login", login);
        cJSON_AddItemToObject(request_log_in, "pass", pass); 
        string = cJSON_Print(request_log_in);
    }
    //send(socket, string , strlen(string) , IP );
    return string;
}
