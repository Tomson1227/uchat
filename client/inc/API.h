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
    SUCCESS = 0,
    ERROR,
    LOGIN_WRONG_USER,
    LOGIN_WRONG_PASS,
    SIGNUP_USER_EXIST,
    ROOM_DOES_NOT_EXIST
}            t_rs_status;

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
