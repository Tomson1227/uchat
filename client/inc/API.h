#ifndef API_H
#define API_H

#include "sqlite3.h"
#include <stdio.h>
#include "cJSON.h"

typedef enum s_api {
    LOGIN,
    SIGNUP,
    CREATE_ROOM,
    SND_MSG,
    READ_MSG,
    SEARCH_USER,
    DELETE_ROOM,
    DELETE_MSG,
    EDIT_MSG,
    OLD_DIALOGS
}            t_api;

typedef enum s_rs_status {
    LOGIN_OK,
    LOGIN_WRONG_USER,
    LOGIN_WRONG_PASS,
    SIGNUP_OK,
    SIGNUP_USER_EXIST,
    SINGUP_FAIL,
    TEXT,
    SND_MSG_OK,
    READ_MSG_OK,
    READ_ALL,
    READ_RECENT
}            t_rs_status;

#endif /* API_H */
