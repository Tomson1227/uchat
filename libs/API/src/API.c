#include "API.h"

/*--- Static fuctions declarations ---*/


/*-------------------------------------*/


/*--- Public functions definitions ---*/
cJSON *receive_rq_log_in_server(const char * const string) {
    cJSON *type = NULL;
    cJSON *login = NULL;
    cJSON *pass = NULL;
    cJSON *rq_log_in = cJSON_Parse(string);
    char *type_parsed = NULL;
    char *login_parsed = NULL;
    char *pass_parsed = NULL;
    int check;

    if (rq_log_in == NULL) {
        const char *error_ptr = cJSON_GetErrorPtr();
        if (error_ptr != NULL) {
            fprintf(stderr, "Error before: %s\n", error_ptr);
        }
        check = 0;
        cJSON_Delete(rq_log_in);
    }
    else {
        type = cJSON_GetObjectItemCaseSensitive(rq_log_in, "type");
        login = cJSON_GetObjectItemCaseSensitive(rq_log_in, "login");
        pass = cJSON_GetObjectItemCaseSensitive(rq_log_in, "pass");
        type_parsed = cJSON_Print(type);
        login_parsed = cJSON_Print(login);
        pass_parsed = cJSON_Print(pass);
        printf("Received type: %s\nReceived login: %s\nReceived password: %s\n", type_parsed, login_parsed, pass_parsed);
    }
    return rq_log_in;
}


cJSON *receive_rs_sign_up_server(const char * const string) {
    cJSON *type = NULL;
    cJSON *login = NULL;
    cJSON *pass = NULL;
    char *type_parsed = NULL;
    char *login_parsed = NULL;
    char *pass_parsed = NULL;
    int check;
    cJSON *rs_sign_up = cJSON_Parse(string);

    if (rs_sign_up == NULL) {
        const char *error_ptr = cJSON_GetErrorPtr();
        if (error_ptr != NULL) {
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
    return rs_sign_up;
}


cJSON *receive_rs_log_in_client(const char * const string) {
    cJSON *type = NULL;
    cJSON *status = NULL;
    cJSON *rs_log_in = cJSON_Parse(string);
    char *type_parsed = NULL;
    char *status_parsed = NULL;
    int check;

    if (rs_log_in == NULL) {
        const char *error_ptr = cJSON_GetErrorPtr();
        if (error_ptr != NULL) {
            fprintf(stderr, "Error before: %s\n", error_ptr);
        }
        check = 0;
        cJSON_Delete(rs_log_in);
    }
    else {
        type = cJSON_GetObjectItemCaseSensitive(rs_log_in, "type");
        status = cJSON_GetObjectItemCaseSensitive(rs_log_in, "status");
        type_parsed = cJSON_Print(type);
        status_parsed = cJSON_Print(status);
        printf("Received type: %s\nReceived status %s\n", type_parsed, status_parsed);
    }
    return rs_log_in;
}


cJSON *receive_rs_sign_up_client(const char * const string) {
    cJSON *type = NULL;
    cJSON *status = NULL;
    cJSON *rs_sign_up = cJSON_Parse(string);
    char *type_parsed = NULL;
    char *status_parsed = NULL;
    int check;

    if (rs_sign_up == NULL) {
        const char *error_ptr = cJSON_GetErrorPtr();
        if (error_ptr != NULL) {
            fprintf(stderr, "Error before: %s\n", error_ptr);
        }
        check = 0;
        cJSON_Delete(rs_sign_up);
    }
    else {
        type = cJSON_GetObjectItemCaseSensitive(rs_sign_up, "type");
        status = cJSON_GetObjectItemCaseSensitive(rs_sign_up, "status");
        type_parsed = cJSON_Print(type);
        status_parsed = cJSON_Print(status);
        printf("Received type: %s\nReceived status %s\n", type_parsed, status_parsed);
    }
    return rs_sign_up;
}


char *send_rq_log_in_client(char *username, char *password) {
    char *string;
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
    char *string;
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


char *send_rs_log_in_server(char *rs_status) {
    char *string;
    cJSON *type = NULL;
    cJSON *status = NULL;
    cJSON *rs_log_in = cJSON_CreateObject();

    if (rs_log_in == NULL) {
        cJSON_Delete(rs_log_in);
    }
    else {
        type = cJSON_CreateString("RESPONSE_LOGIN");
        cJSON_AddItemToObject(rs_log_in, "type", type);
        status = cJSON_CreateString(rs_status);
        cJSON_AddItemToObject(rs_log_in, "status", status);
        string = cJSON_Print(rs_log_in);
    }
    return string;
}


char *send_rs_sign_up_server(char *rs_status) {
    char *string;
    cJSON *type = NULL;
    cJSON *status = NULL;
    cJSON *rs_sign_up = cJSON_CreateObject();

    if (rs_sign_up == NULL) {
        cJSON_Delete(rs_sign_up);
    }
    else {
        type = cJSON_CreateString("RESPONSE_SIGNUP");
        cJSON_AddItemToObject(rs_sign_up, "type", type);
        status = cJSON_CreateString(rs_status);
        cJSON_AddItemToObject(rs_sign_up, "status", status);
        string = cJSON_Print(rs_sign_up);
    }
    return string;
}


/*-------------------------------------*/


/*--- Static fuctions definitions ---*/


/*-------------------------------------*/
