//#include "../libraries/cJSON/cJSON.h"
#include <stdio.h>

/*SIGNUP
    From server to client response {
    "type": RESPONSE_SIGNUP,
    "status": SINGUP_OK | SIGNUP_USER_EXIST | SINGUP_FAIL,
}*/

void receive_rs_log_in(const char * const string)
{
    //receiving signup responce from server
    // recv(sock, buffer, buffer_size, IP);

    cJSON *type = NULL;
    cJSON *status = NULL;
    cJSON *rs_sign_up = cJSON_Parse(string);
    char *type_parsed = NULL;
    char *status_parsed = NULL;
    int check;

    if (rs_sign_up == NULL)
    {
        const char *error_ptr = cJSON_GetErrorPtr();
        if (error_ptr != NULL)
        {
            fprintf(stderr, "Error before: %s\n", error_ptr);
        }
        check = 0;
        goto end;
    }

    type = cJSON_GetObjectItemCaseSensitive(rs_sign_up, "type");
    status = cJSON_GetObjectItemCaseSensitive(rs_sign_up, "status");
    type_parsed = cJSON_Print(type);
    status_parsed = cJSON_Print(status);
    printf("Received type: %s\nReceived status %s\n", type_parsed, status_parsed);

    end:
        cJSON_Delete(rs_sign_up);
}
