#ifndef API_H
#define API_H

#include "sqlite3.h"
#include <stdio.h>
#include "cJSON.h"
#include <string.h>

typedef enum s_api
{
    LOGIN,
    SIGNUP,
    CREATE_ROOM,
    CREATE_MSG,
    SND_MSG
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

#endif /* API_H */
