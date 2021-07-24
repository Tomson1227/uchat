#include "API.h"

/*--- Static fuctions declarations ---*/
/*-------------------------------------*/
/*--- Public functions definitions ---*/

void receive_rs_log_in_server(const char * const string) 
{
    cJSON *login = NULL;
    cJSON *pass = NULL;
    cJSON *rq_log_in = cJSON_Parse(string);
    char *login_parsed = NULL;
    char *pass_parsed = NULL;

    if (rq_log_in == NULL) {
        const char *error_ptr = cJSON_GetErrorPtr();
        if (error_ptr != NULL) {
            fprintf(stderr, "Error before: %s\n", error_ptr);
        }
        cJSON_Delete(rq_log_in);
    }
    else {
        login = cJSON_GetObjectItemCaseSensitive(rq_log_in, "login");
        pass = cJSON_GetObjectItemCaseSensitive(rq_log_in, "pass");
        login_parsed = cJSON_Print(login);
        pass_parsed = cJSON_Print(pass);

        printf("Received login: %s\tReceived password: %s\n", login->valuestring, pass->valuestring);
    }

    cJSON_Delete(rq_log_in);
}

void receive_rs_sign_up_server(const char * const string) {
    cJSON *login = NULL;
    cJSON *pass = NULL;
    char *login_parsed = NULL;
    char *pass_parsed = NULL;
    cJSON *rs_sign_up = cJSON_Parse(string);

    if (rs_sign_up == NULL) {
        const char *error_ptr = cJSON_GetErrorPtr();
        if (error_ptr != NULL) {
            fprintf(stderr, "Error before: %s\n", error_ptr);
        }
        cJSON_Delete(rs_sign_up);
    }
    else {
        login = cJSON_GetObjectItemCaseSensitive(rs_sign_up, "login");
        pass = cJSON_GetObjectItemCaseSensitive(rs_sign_up, "pass");
        login_parsed = cJSON_Print(login);
        pass_parsed = cJSON_Print(pass);

        printf("Received login %s\nReceived pass: %s\n", login_parsed, pass_parsed);
    }

    cJSON_Delete(rs_sign_up);
}

cJSON *receive_rs_log_in_client(const char * const string) {
    cJSON *status = NULL;
    cJSON *rs_log_in = cJSON_Parse(string);
    char *status_parsed = NULL;

    if (rs_log_in == NULL) {
        const char *error_ptr = cJSON_GetErrorPtr();
        if (error_ptr != NULL) {
            fprintf(stderr, "Error before: %s\n", error_ptr);
        }
        cJSON_Delete(rs_log_in);
    }
    else {
        status = cJSON_GetObjectItemCaseSensitive(rs_log_in, "status");
        status_parsed = cJSON_Print(status);
        printf("Received status %s\n", status_parsed);
    }
    return rs_log_in;
}

cJSON *receive_rs_sign_up_client(const char * const string) {
    cJSON *status = NULL;
    cJSON *rs_sign_up = cJSON_Parse(string);
    char *status_parsed = NULL;

    if (rs_sign_up == NULL) {
        const char *error_ptr = cJSON_GetErrorPtr();
        if (error_ptr != NULL) {
            fprintf(stderr, "Error before: %s\n", error_ptr);
        }
        cJSON_Delete(rs_sign_up);
    }
    else {
        status = cJSON_GetObjectItemCaseSensitive(rs_sign_up, "status");
        status_parsed = cJSON_Print(status);
        printf("Received status %s\n", status_parsed);
    }
    return rs_sign_up;
}

char *send_rq_log_in_client(char *username, char *password) {
    char *string = NULL;
    cJSON *type = NULL;
    cJSON *login = NULL;
    cJSON *pass = NULL;
    cJSON *request_log_in = cJSON_CreateObject();

    if (request_log_in == NULL) {
        cJSON_Delete(request_log_in);
    }
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
    return string;
}

char *send_rq_sign_in_client(char *username, char *password) {
    char *string = NULL;
    cJSON *type = NULL;
    cJSON *login = NULL;
    cJSON *pass = NULL;
    cJSON *request_sign_up = cJSON_CreateObject();

    if (request_sign_up == NULL) {
        cJSON_Delete(request_sign_up);
    }
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
    return string;
}

char *send_rs_log_in_server(t_response_status response) {
    char *string = NULL;
    cJSON *type = NULL;
    cJSON *status = NULL;
    cJSON *rs_log_in = cJSON_CreateObject();

    if (rs_log_in == NULL) {
        cJSON_Delete(rs_log_in);
    }
    else {
        type = cJSON_CreateString("RESPONSE_LOGIN");
        cJSON_AddItemToObject(rs_log_in, "type", type);        
        
        status = cJSON_CreateNumber(response);
        cJSON_AddItemToObject(rs_log_in, "status", status);
        string = cJSON_Print(rs_log_in);
    }
    
    cJSON_Delete(rs_log_in);
    return string;
}

char *send_rs_sign_up_server(t_response_status response) {
    char *string = NULL;
    cJSON *type = NULL;
    cJSON *status = NULL;
    cJSON *rs_sign_up = cJSON_CreateObject();

    if (rs_sign_up == NULL) {
        cJSON_Delete(rs_sign_up);
    }
    else {
        type = cJSON_CreateString("RESPONSE_SIGNUP");
        cJSON_AddItemToObject(rs_sign_up, "type", type);
        status = cJSON_CreateNumber(response);
        cJSON_AddItemToObject(rs_sign_up, "status", status);
        string = cJSON_Print(rs_sign_up);
    }
    return string;
}

void define_rq_type(const char * const string) {
    cJSON *type = NULL;
    cJSON *rq = cJSON_Parse(string);
    char *type_parsed;

    if (rq == NULL) {
        const char *error_ptr = cJSON_GetErrorPtr();
        if (error_ptr != NULL) {
            fprintf(stderr, "Error before: %s\n", error_ptr);
        }
        cJSON_Delete(rq);
    }
    type = cJSON_GetObjectItemCaseSensitive(rq, "type");
    type_parsed = cJSON_Print(type);
    if (strcmp(type_parsed, "\"REQUEST_LOGIN\"") == 0)
        receive_rs_log_in_server(string);
    if (strcmp(type_parsed, "\"REQUEST_SIGNUP\"") == 0) 
        receive_rs_sign_up_server(string);
    if (strcmp(type_parsed, "\"RESPONSE_LOGIN\"") == 0) 
        receive_rs_log_in_client(string);
    if (strcmp(type_parsed, "\"RESPONSE_SIGNUP\"") == 0) 
        receive_rs_sign_up_client(string);
}

/*-------------------------------------*/
/*--- Static fuctions definitions ---*/
/*-------------------------------------*/
