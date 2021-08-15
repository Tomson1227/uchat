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

typedef enum s_rs_login_status {
    SUCCESS = 0,
    LOGIN_WRONG_USER = 1,
    LOGIN_WRONG_PASS = 2,
    ERROR = 3,
    ROOM_DOES_NOT_EXIST = 4
}            t_rs_login_status;

typedef enum s_rs_signup_status {
    SIGNUP_SUCCESS = 3,
    SIGNUP_USER_EXIST = 4,
    SIGNUP_ERROR = 5
}           t_rs_signup_status;

typedef enum s_update {
    TRUE = 0,
    FALSE
}            t_update;

typedef enum s_msg_type {
    MESSAGE,
    FILE_M,
    IMAGE
}            t_msg_type;

#endif /* API_H */
