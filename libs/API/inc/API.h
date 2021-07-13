#ifndef API_H
#define API_H

#include <stdio.h>
#include "cJSON.h"
#include <string.h>

typedef enum s_request_type
{
    REQUEST_LOGIN,
    REQUEST_SIGNUP
} t_request_type;

typedef enum s_response_type
{
    RESPONSE_LOGIN,
    RESPONSE_SIGNUP
} t_response_type;

typedef enum s_request_login_type
{
    LOGIN_OK,
    LOGIN_WRONG_USER,
    LOGIN_WRONG_PASS
}   t_request_login_type;

typedef enum s_response_login_type
{
    SIGNUP_OK,
    SIGNUP_USER_EXIST,
    SINGUP_FAIL
}   t_response_login_type;

void define_rq_type(const char * const string);
cJSON *receive_rq_log_in_server(const char * const string);
cJSON *receive_rs_sign_up_server(const char * const string);
cJSON *receive_rs_log_in_client(const char * const string);
cJSON *receive_rs_sign_up_client(const char * const string);
char *send_rq_log_in_client(char *username, char *password);
char *send_rq_sign_in_client(char *username, char *password);
char *send_rs_log_in_server(char *rs_status);
char *send_rs_sign_up_server(char *rs_status);

#endif /* API_H */
