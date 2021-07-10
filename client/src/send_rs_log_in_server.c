#include "cJSON.h"
#include <stdio.h>

/*LOGIN
    response from server to client{
    "type": RESPONSE_LOGIN,
    "status": "LOGIN_OK"
}*/

char *send_rs_log_in_server(char *rs_status) {
    char *string;
    cJSON *type = NULL;
    cJSON *status = NULL;
    cJSON *rs_log_in = cJSON_CreateObject();

    if (rs_log_in == NULL)
        cJSON_Delete(rs_log_in);
    else {
        type = cJSON_CreateString("RESPONSE_LOGIN");
        cJSON_AddItemToObject(rs_log_in, "type", type);
        status = cJSON_CreateString(rs_status);
        cJSON_AddItemToObject(rs_log_in, "status", status);
        string = cJSON_Print(rs_log_in);
    }
    //send(socket, string , strlen(string) , IP );
    return string;
}
