#define __STDC_WANT_LIB_EXT1__ 1

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <locale.h>
#include <string.h>
#include <sys/poll.h>
#include <sys/socket.h> 
#include <sys/errno.h>

#ifdef __linux__
    #include <malloc.h> // For Linux
    #define malloc_size malloc_usable_size
#else
    #include <malloc/malloc.h> //For macOS
#endif

typedef enum e_struct_type {
    login_request,
    signup_request
}            t_struct_type;

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

typedef struct s_user_request {
    t_api request;
    char *user_login;
    char *password;
}              t_user_request;

typedef struct s_server_response {
    t_api response;
    t_status status;
}              t_server_responce;


char *read_socket(int fd);
void *read_response(char *line);
int server_responce(t_server_responce *response, int fd);
char *login_convert(t_user_request *request);
int process_request(char *request, int fd);
