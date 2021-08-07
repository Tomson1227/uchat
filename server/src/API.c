#include "uchat_server.h"

/*--- Static fuctions declarations ---*/
static void check_error(void);
static void receive_rq_log_in_server(cJSON *rq, sqlite3 *db);
static void receive_rq_sign_up_server(cJSON *rq, sqlite3 *db);
static void send_rs_sign_up_server(t_rs_status response);
static void send_rs_log_in_server(t_rs_status response);
/*-------------------------------------*/
/*--- Public functions definitions ---*/

void process_rq_server(const char *const string, sqlite3 *db)
{
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

static void receive_rq_log_in_server(cJSON *rq, sqlite3 *db)
{
    cJSON *log_in = cJSON_GetObjectItemCaseSensitive(rq, "login");
    cJSON *pass = cJSON_GetObjectItemCaseSensitive(rq, "pass");

    t_rs_status status = sign_up(db, log_in->valuestring, pass->valuestring);
    send_rs_sign_up_server(status);
}

static void receive_rq_sign_up_server(cJSON *rq, sqlite3 *db)
{
    cJSON *log_in = cJSON_GetObjectItemCaseSensitive(rq, "login");
    cJSON *pass = cJSON_GetObjectItemCaseSensitive(rq, "pass");

    t_rs_status status = login(db, log_in->valuestring, pass->valuestring);
    send_rs_log_in_server(status);
}

static void send_rs_log_in_server(t_rs_status response)
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

        send_message(message);

        free(message);
        cJSON_Delete(rs_log_in);
    }
    else
        check_error();
}

static void send_rs_sign_up_server(t_rs_status response)
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
        
        send_message(message);

        free(message);
        cJSON_Delete(rs_sign_up);
    }
    else
        check_error();
}

static void check_error(void)
{
    const char *error_ptr = cJSON_GetErrorPtr();

    if (error_ptr != NULL)
        fprintf(stderr, "Error before: %s\n", error_ptr);
}
