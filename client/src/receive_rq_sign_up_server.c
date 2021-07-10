
#include "cJSON.h"
#include <stdio.h>

/*SIGNUP
    Receiving request from client to server{
    "type": REQUEST_SIGNUP,
    "login": "user_login",
    "pass": "user_pass",
}*/

void receive_rs_sign_up_server(const char * const string)
{
    //receiving signup responce from server
    // recv(sock, buffer, buffer_size, IP);
    cJSON *type = NULL;
    cJSON *login = NULL;
    cJSON *pass = NULL;
    char *type_parsed = NULL;
    char *login_parsed = NULL;
    char *pass_parsed = NULL;
    int check;
    cJSON *rs_sign_up = cJSON_Parse(string);

    if (rs_sign_up == NULL)
    {
        const char *error_ptr = cJSON_GetErrorPtr();
        if (error_ptr != NULL)
        {
            fprintf(stderr, "Error before: %s\n", error_ptr);
        }
        check = 0;
        cJSON_Delete(rs_sign_up);
    }
    else {
        type = cJSON_GetObjectItemCaseSensitive(rs_sign_up, "type");
        login = cJSON_GetObjectItemCaseSensitive(rs_sign_up, "login");
        pass = cJSON_GetObjectItemCaseSensitive(rs_sign_up, "pass");
        type_parsed = cJSON_Print(type);
        login_parsed = cJSON_Print(login);
        pass_parsed = cJSON_Print(pass);
        printf("Received type: %s\nReceived login %s\nReceived pass: %s\n", type_parsed, login_parsed, pass_parsed);
    }
}
