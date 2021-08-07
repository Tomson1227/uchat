#include "uchat_server.h"

/*--- Static fuctions declarations ---*/
static void check_error(void);
static void receive_rq_log_in_server(cJSON *rq, sqlite3 *db, int fd);
static void receive_rq_sign_up_server(cJSON *rq, sqlite3 *db, int fd);
static void send_rs_sign_up_server(t_rs_status response, int fd);
static void send_rs_log_in_server(t_rs_status response, int fd);
/*-------------------------------------*/
/*--- Public functions definitions ---*/

void process_rq_server(const char *const string, sqlite3 *db, int fd) {
    printf("API: %s\n", string);
    cJSON *rq = NULL;

    if ((rq = cJSON_Parse(string)))
    {
        cJSON *type = cJSON_GetObjectItemCaseSensitive(rq, "type");
        // char *tmp = cJSON_Print(type);
        
        printf("%d\n", (int)type->valuedouble);

        switch ((int)type->valuedouble)
        {
        case LOGIN:
            receive_rq_log_in_server(rq, db, fd);
            break;
        case SIGNUP:
            receive_rq_sign_up_server(rq, db, fd);
            break;
        case CREATE_ROOM:
            break;
        case CREATE_MSG:
            break;
        case SND_MSG:
            break;
        default:
            /* UNKNOWN REQUEST */
            break;
        }
        cJSON_Delete(rq);
    }
    else
        check_error();
}

/*-------------------------------------*/
/*--- Static fuctions definitions ---*/
/*-------------------------------------*/

static void receive_rq_log_in_server(cJSON *rq, sqlite3 *db, int fd) {
    cJSON *log_in = cJSON_GetObjectItemCaseSensitive(rq, "login");
    cJSON *pass = cJSON_GetObjectItemCaseSensitive(rq, "pass");

    t_rs_status status = login(db, log_in->valuestring, pass->valuestring);
    send_rs_log_in_server(status, fd);
}

static void receive_rq_sign_up_server(cJSON *rq, sqlite3 *db, int fd) {
    cJSON *log_in = cJSON_GetObjectItemCaseSensitive(rq, "login");
    cJSON *pass = cJSON_GetObjectItemCaseSensitive(rq, "pass");

    t_rs_status status = sign_up(db, log_in->valuestring, pass->valuestring);
    // printf("log_in: %s\npass: %s\n", log_in->valuestring, pass->valuestring);
    // printf("%d\n", status);
    send_rs_sign_up_server(status, fd);
    // printf("success\n");
}

static void send_rs_log_in_server(t_rs_status response, int fd)
{
    char *message = NULL;
    cJSON *rs_log_in = NULL;

    if ((rs_log_in = cJSON_CreateObject()))
    {
        cJSON *type = cJSON_CreateNumber(LOGIN);
        cJSON *status = cJSON_CreateNumber(response);

        cJSON_AddItemToObject(rs_log_in, "type", type);
        cJSON_AddItemToObject(rs_log_in, "status", status);

        message = cJSON_Print(rs_log_in);
        // printf("%s\n", message);
         
        if(send(fd, message, strlen(message), 0) < 0)
            perror("send fail");

        // printf("message sent\n");
        free(message);
        cJSON_Delete(rs_log_in);
    }
    else
        check_error();
}

static void send_rs_sign_up_server(t_rs_status response, int fd)
{
    char *message = NULL;
    cJSON *rs_sign_up = NULL;

    if ((rs_sign_up = cJSON_CreateObject()))
    {
        cJSON *type = cJSON_CreateNumber(SIGNUP);
        cJSON *status = cJSON_CreateNumber(response);

        cJSON_AddItemToObject(rs_sign_up, "type", type);
        cJSON_AddItemToObject(rs_sign_up, "status", status);
        message = cJSON_Print(rs_sign_up);
        printf("%s\n", message);
        if(send(fd, message, strlen(message), 0) < 0)
            perror("send fail");

        free(message);
        cJSON_Delete(rs_sign_up);
    }
    else
        check_error();
}

static void check_error(void) {
    const char *error_ptr = cJSON_GetErrorPtr();

    if (error_ptr != NULL)
        fprintf(stderr, "Error before: %s\n", error_ptr);
}
