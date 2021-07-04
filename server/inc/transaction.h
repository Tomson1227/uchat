#define __STDC_WANT_LIB_EXT1__ 1

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <locale.h>

#ifdef __linux__
    #include <malloc.h> // For Linux
    #define malloc_size malloc_usable_size
#else
    #include <malloc/malloc.h> //For macOS
#endif

typedef enum e_api {
    REQUEST_LOGIN,
    REQUEST_SIGNUP    
}            t_api;

typedef enum e_status {
    LOGIN_OK,
    LOGIN_WRONG_USER,
    LOGIN_WRONG_PASSWORD,
    SINGUP_OK,
    SINGUP_USER_EXIST,
    SINGUP_FAIL
}            t_status;

typedef struct s_login_request {
    t_api request;
    char *user_logit;
    char *password;
}              t_login_request;

#define t_signup_request  t_login_request

typedef struct s_server_response {
    t_api response;
    t_status status;
}              t_server_responce;


void *read_response(char *line);
char *server_responce(t_server_responce *response);
char *login_convert(t_login_request *request);
