#include "uchat_server.h"

/*--- Static fuctions declarations ---*/
static void check_error(void);
/*-------------------------------------*/
/*--- Public functions definitions ---*/

void receive_rs_log_in_server(const char * const string) {
    cJSON *rq_log_in = NULL;

    if ((rq_log_in = cJSON_Parse(string))) {
        cJSON *login = cJSON_GetObjectItemCaseSensitive(rq_log_in, "login");
        cJSON *pass = cJSON_GetObjectItemCaseSensitive(rq_log_in, "pass");

        //use login->valuestring && pass->valuestring
        /* SERVER PROCESS REQUEST */
        /* SERVER SEND RESPONCE */

        cJSON_Delete(rq_log_in);
    } else
        check_error();
}

void receive_rs_sign_up_server(const char * const string) {
    cJSON *rs_sign_up = NULL;

    if ((rs_sign_up = cJSON_Parse(string))) {
        cJSON *login = cJSON_GetObjectItemCaseSensitive(rs_sign_up, "login");
        cJSON *pass = cJSON_GetObjectItemCaseSensitive(rs_sign_up, "pass");

        //use login->valuestring && pass->valuestring
        /* SERVER PROCESS REQUEST */
        /* SERVER SEND RESPONCE */

        cJSON_Delete(rs_sign_up);
    }
    else 
        check_error();
}

cJSON *receive_rs_log_in_client(const char * const string) {
    cJSON *rs_log_in = NULL;

    if ((rs_log_in = cJSON_Parse(string))) {
        cJSON *status = status = cJSON_GetObjectItemCaseSensitive(rs_log_in, "status");
        printf("Received status %s\n", status->valuestring);
    } else 
        check_error();

    return rs_log_in;
}

cJSON *receive_rs_sign_up_client(const char * const string) {
    cJSON *rs_sign_up = NULL;

    if ((rs_sign_up = cJSON_Parse(string))) {
        cJSON *status = cJSON_GetObjectItemCaseSensitive(rs_sign_up, "status");
        printf("Received status %s\n", status->valuestring);
    } else {
        const char *error_ptr = cJSON_GetErrorPtr();

        if (error_ptr != NULL)
            fprintf(stderr, "Error before: %s\n", error_ptr);
    }

    return rs_sign_up;
}

char *send_rq_log_in_client(char *username, char *password) {
    char *string = NULL;
    cJSON *request_log_in = NULL;

    if ((request_log_in = cJSON_CreateObject())) {
        cJSON *type = cJSON_CreateString("REQUEST_LOGIN");
        cJSON *login = cJSON_CreateString(username);
        cJSON *pass = cJSON_CreateString(password);

        cJSON_AddItemToObject(request_log_in, "type", type);
        cJSON_AddItemToObject(request_log_in, "login", login);
        cJSON_AddItemToObject(request_log_in, "pass", pass); 

        string = cJSON_Print(request_log_in);
        cJSON_Delete(request_log_in);
    }

    return string;
}

char *send_rq_sign_in_client(char *username, char *password) {
    char *string = NULL;
    cJSON *request_sign_up = NULL;

    if ((request_sign_up = cJSON_CreateObject())) {
        cJSON *type = cJSON_CreateString("REQUEST_SIGNUP");
        cJSON *login = cJSON_CreateString(username);
        cJSON *pass = cJSON_CreateString(password);

        cJSON_AddItemToObject(request_sign_up, "type", type);
        cJSON_AddItemToObject(request_sign_up, "login", login);
        cJSON_AddItemToObject(request_sign_up, "pass", pass); 

        string = cJSON_Print(request_sign_up);
        cJSON_Delete(request_sign_up);
    } else  
        check_error();

    return string;
}

char *send_rs_log_in_server(t_response_status response) {
    char *string = NULL;
    cJSON *rs_log_in = NULL;

    if ((rs_log_in = cJSON_CreateObject())) {
        cJSON *type = cJSON_CreateString("RESPONSE_LOGIN");
        cJSON *status = cJSON_CreateNumber(response);

        cJSON_AddItemToObject(rs_log_in, "type", type);        
        cJSON_AddItemToObject(rs_log_in, "status", status);

        string = cJSON_Print(rs_log_in);
        cJSON_Delete(rs_log_in);
    } else
        check_error();
    
    return string;
}

char *send_rs_sign_up_server(t_response_status response) {
    char *string = NULL;
    cJSON *rs_sign_up = NULL;

    if ((rs_sign_up = cJSON_CreateObject())) {
        cJSON *type = cJSON_CreateString("RESPONSE_SIGNUP");
        cJSON *status = cJSON_CreateNumber(response);

        cJSON_AddItemToObject(rs_sign_up, "type", type);
        cJSON_AddItemToObject(rs_sign_up, "status", status);
        
        string = cJSON_Print(rs_sign_up);
        cJSON_Delete(rs_sign_up);
    } else 
        check_error();

    return string;
}

void define_rq_type(const char * const string) {
    cJSON *rq = NULL;

    if ((rq = cJSON_Parse(string))) {
        cJSON *type = cJSON_GetObjectItemCaseSensitive(rq, "type");
        
        if (strcmp(type->valuestring, "\"REQUEST_LOGIN\"") == 0)
            receive_rs_log_in_server(string);
        else if (strcmp(type->valuestring, "\"REQUEST_SIGNUP\"") == 0) 
            receive_rs_sign_up_server(string);
        else if (strcmp(type->valuestring, "\"RESPONSE_LOGIN\"") == 0) 
            receive_rs_log_in_client(string);
        else if (strcmp(type->valuestring, "\"RESPONSE_SIGNUP\"") == 0) 
            receive_rs_sign_up_client(string);
    
        cJSON_Delete(rq);   
    } else
        check_error();
}

/*-------------------------------------*/
/*--- Static fuctions definitions ---*/
/*-------------------------------------*/

static void check_error(void) {
    const char *error_ptr = cJSON_GetErrorPtr();
    
    if (error_ptr != NULL)
        fprintf(stderr, "Error before: %s\n", error_ptr);
}
