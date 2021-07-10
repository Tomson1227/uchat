#ifndef API_H
#define API_H

#include <stdio.h>
#include "cJSON.h"

cJSON *receive_rq_log_in_server(const char * const string);
cJSON *receive_rs_sign_up_server(const char * const string);
cJSON *receive_rs_log_in_client(const char * const string);
cJSON *receive_rs_sign_up_client(const char * const string);
char *send_rq_log_in_client(char *username, char *password);
char *send_rq_sign_in_client(char *username, char *password);
char *send_rs_log_in_server(char *rs_status);
char *send_rs_sign_up_server(char *rs_status);


#endif /* API_H */
