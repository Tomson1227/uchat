#include "cJSON.h"
#include <stdio.h>

/*LOGIN
    From server to client response {
    "type": RESPONSE_LOGIN,
    "status": LOGIN_OK
}*/

void receive_rs_log_in_client(const char * const string)
{
    //receiving login responce from server
    // recv(sock, buffer, buffer_size, IP);

    cJSON *type = NULL;
    cJSON *status = NULL;
    cJSON *rs_log_in = cJSON_Parse(string);
    char *type_parsed = NULL;
    char *status_parsed = NULL;
    int check;

    if (rs_log_in == NULL)
    {
        const char *error_ptr = cJSON_GetErrorPtr();
        if (error_ptr != NULL)
        {
            fprintf(stderr, "Error before: %s\n", error_ptr);
        }
        check = 0;
        cJSON_Delete(rs_log_in);
    }
    else  {
        type = cJSON_GetObjectItemCaseSensitive(rs_log_in, "type");
        status = cJSON_GetObjectItemCaseSensitive(rs_log_in, "status");
        type_parsed = cJSON_Print(type);
        status_parsed = cJSON_Print(status);
        printf("Received type: %s\nReceived status %s\n", type_parsed, status_parsed);
    } 
}
