#include "cJSON.h"
#include <stdio.h>

/*SGINUP
    sending response from server to client{
    "type": RESPONSE_SIGNUP,
    "status": SINGUP_OK | SIGNUP_USER_EXIST | SINGUP_FAIL,
}*/

char *send_rs_sign_up_server(char *rs_status)
{
    char *string;
    cJSON *type = NULL;
    cJSON *status = NULL;
    cJSON *rs_sign_up = cJSON_CreateObject();

    if (rs_sign_up == NULL)
        cJSON_Delete(rs_sign_up);
    else {
        type = cJSON_CreateString("RESPONSE_SIGNUP");
        cJSON_AddItemToObject(rs_sign_up, "type", type);
        status = cJSON_CreateString(rs_status);
        cJSON_AddItemToObject(rs_sign_up, "status", status);
        string = cJSON_Print(rs_sign_up);
    }
    //send(socket, string , strlen(string) , IP );
    return string;
}
