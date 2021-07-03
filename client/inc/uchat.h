#pragma once

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>

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

typedef enum s_response_login_type
{
    LOGIN_OK,
    LOGIN_WRONG_USER,
    LOGIN_WRONG_PASS
}   t_response_login_type;

typedef enum s_response_login_type
{
    SIGNUP_OK,
    SIGNUP_USER_EXIST,
    SINGUP_FAIL
}   t_response_login_type;

//sending requsets functions

//receiving responses functions
