#ifndef API_H
#define API_H

#include "sqlite3.h"
#include <stdio.h>
#include "cJSON.h"
#include <string.h>
#include "sqlite3.h"

typedef enum s_api
{
    LOGIN,
    SIGNUP
}            t_api;

typedef enum s_rs_status
{
    LOGIN_OK,
    LOGIN_WRONG_USER,
    LOGIN_WRONG_PASS,
    SIGNUP_OK,
    SIGNUP_USER_EXIST,
    SINGUP_FAIL
}            t_rs_status;

cJSON *receive_rs_log_in_client(const char * const string);
cJSON *receive_rs_sign_up_client(const char * const string);
char *send_rq_log_in_client(char *username, char *password);
char *send_rq_sign_in_client(char *username, char *password);

//Server function
void process_rq_server(const char *const string, sqlite3 *db);

#endif /* API_H */
