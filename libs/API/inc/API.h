#ifndef API_H
#define API_H

#include <stdio.h>
#include "cJSON.h"
#include <string.h>

typedef enum s_api
{
    LOGIN,
    SIGNUP
} t_api;

typedef enum s_response_status
{
    LOGIN_OK,
    LOGIN_WRONG_USER,
    LOGIN_WRONG_PASS,
    SIGNUP_OK,
    SIGNUP_USER_EXIST,
    SINGUP_FAIL
}   t_response_status;

void define_rq_type(const char * const string);
cJSON *receive_rq_log_in_server(const char * const string);
cJSON *receive_rs_sign_up_server(const char * const string);
cJSON *receive_rs_log_in_client(const char * const string);
cJSON *receive_rs_sign_up_client(const char * const string);
char *send_rq_log_in_client(char *username, char *password);
char *send_rq_sign_in_client(char *username, char *password);
char *send_rs_log_in_server(t_response_status response);
char *send_rs_sign_up_server(t_response_status response);

#endif /* API_H */
