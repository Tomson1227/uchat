#include "uchat_server.h"

/*--- Static fuctions declarations ---*/
static void check_error(void);
static void receive_rq_sign_up_server(cJSON *rq, sqlite3 *db);
static void receive_rq_log_in_server(cJSON *rq, sqlite3 *db);
static char *send_rs_sign_up_server(t_rs_status response);
static char *send_rs_log_in_server(t_rs_status response);
static void receive_rs_log_in_client(cJSON *rs);
static void receive_rs_sign_up_client(cJSON *rs);
/*-------------------------------------*/
/*--- Public functions definitions ---*/

void process_rq_server(const char *const string, sqlite3 *db) {
    cJSON *rq = NULL;

    if ((rq = cJSON_Parse(string)))
    {
        cJSON *type = cJSON_GetObjectItemCaseSensitive(rq, "type");

        printf("API: %s\n", type->valuestring);

        switch ((int)type->valuedouble)
        {
        case LOGIN:
            receive_rq_log_in_server(rq, db);
            break;
        case SIGNUP:
            receive_rq_sign_up_server(rq, db);
            break;
        default:
            /* UNKNOWN REQUEST */
            break;
        }
        // printf("Allocation? %s\n", malloc_size(rq) ? "Yes" : "No");
        cJSON_Delete(rq);
        // printf("Allocation? %s\n", malloc_size(rq) ? "Yes" : "No");
    }
    else
        check_error();
}

/*-------------------------------------*/
/*--- Static fuctions definitions ---*/
/*-------------------------------------*/

static void receive_rq_log_in_server(cJSON *rq, sqlite3 *db) {
    cJSON *log_in = cJSON_GetObjectItemCaseSensitive(rq, "login");
    cJSON *pass = cJSON_GetObjectItemCaseSensitive(rq, "pass");

    // t_rs_status status = sign_up(db, log_in->valuestring, pass->valuestring);
    /* SERVER PROCESS REQUEST */
    /* SERVER SEND RESPONCE */
}

static void receive_rq_sign_up_server(cJSON *rq, sqlite3 *db) {
    cJSON *log_in = cJSON_GetObjectItemCaseSensitive(rq, "login");
    cJSON *pass = cJSON_GetObjectItemCaseSensitive(rq, "pass");

    // t_rs_status status = login(db, log_in->valuestring, pass->valuestring);
    /* SERVER PROCESS REQUEST */
    /* SERVER SEND RESPONCE */
}

static char *send_rs_log_in_server(t_rs_status response) {
    char *string = NULL;
    cJSON *rs_log_in = NULL;

    if ((rs_log_in = cJSON_CreateObject()))
    {
        cJSON *type = cJSON_CreateNumber(LOGIN);
        cJSON *status = cJSON_CreateNumber(response);

        cJSON_AddItemToObject(rs_log_in, "type", type);
        cJSON_AddItemToObject(rs_log_in, "status", status);

        string = cJSON_Print(rs_log_in);
        cJSON_Delete(rs_log_in);
    }
    else
        check_error();

    return string;
}

static char *send_rs_sign_up_server(t_rs_status response) {
    char *string = NULL;
    cJSON *rs_sign_up = NULL;

    if ((rs_sign_up = cJSON_CreateObject()))
    {
        cJSON *type = cJSON_CreateNumber(SIGNUP);
        cJSON *status = cJSON_CreateNumber(response);

        cJSON_AddItemToObject(rs_sign_up, "type", type);
        cJSON_AddItemToObject(rs_sign_up, "status", status);

        string = cJSON_Print(rs_sign_up);
        cJSON_Delete(rs_sign_up);
    }
    else
        check_error();

    return string;
}

static void check_error(void) {
    const char *error_ptr = cJSON_GetErrorPtr();

    if (error_ptr != NULL)
        fprintf(stderr, "Error before: %s\n", error_ptr);
}
