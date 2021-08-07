#include "uchat.h"

/*--- Static fuctions declarations ---*/
static void check_error(void);
static void receive_rs_log_in_client(cJSON *rs, t_chat *chat);
static void receive_rs_sign_up_client(cJSON *rs, t_chat *chat);
static void receive_rs_create_msg_client(cJSON *rs, t_chat *chat);
static void receive_rs_create_room_client(cJSON *rs, t_chat *chat);

/*-------------------------------------*/
/*--- Public functions definitions ---*/

void process_rs_client(const char *const string, t_chat *chat) {
    cJSON *rq = NULL;

    if ((rq = cJSON_Parse(string)))
    {
        cJSON *type = cJSON_GetObjectItemCaseSensitive(rq, "type");

        switch ((int)type->valuedouble)
        {
        case LOGIN:
            receive_rs_log_in_client(rq, chat);
            break;
        case SIGNUP:
            receive_rs_sign_up_client(rq, chat);
            break;
        case CREATE_ROOM:
            receive_rs_create_room_client(rq, chat);
            break;
        case CREATE_MSG:
            receive_rs_create_msg_client(rq, chat);
            break;
        case SND_MSG:

            break;
        default:
            /* UNKNOWN RESPONSE */
            break;
        }
        cJSON_Delete(rq);
    }
    else
        check_error();
}

static void receive_rs_log_in_client(cJSON *rs, t_chat *chat) {
    cJSON *status = status = cJSON_GetObjectItemCaseSensitive(rs, "status");
    GObject *wnd_main = gtk_builder_get_object(chat->builder, "window_main");

    /* PROCESS RESPONSE */
    switch((int)status->valuedouble) {
        case LOGIN_OK:
            gtk_widget_hide(GTK_WIDGET(wnd_main));
            init_chat_window(chat);
            break;
        case LOGIN_WRONG_USER:
            display_error_wrong_username_login(chat);
            break;
        case LOGIN_WRONG_PASS:
            display_error_wrong_password_login(chat);
            break;
        default:
            /* UNKNOWN STATUS */
            break;
    }
}

static void receive_rs_sign_up_client(cJSON *rs, t_chat *chat) {
    cJSON *status = cJSON_GetObjectItemCaseSensitive(rs, "status");
    GObject *wnd_main = gtk_builder_get_object(chat->builder, "window_main");
    
    /* PROCESS RESPONSE */
    switch((int) status->valuedouble) {
        case SIGNUP_OK:
            gtk_widget_hide(GTK_WIDGET(wnd_main));
            init_chat_window(chat);
            break;
        case SIGNUP_USER_EXIST:
            display_error_user_exists(chat);
            break;
        case SINGUP_FAIL:

            break;
        default:
            /*UNKNOWN STATUS*/
            break;
    }
}

char *send_rq_create_room_client(char *username, char *customer) {
    char *string = NULL;
    cJSON *request_create_room = NULL;

    if ((request_create_room = cJSON_CreateObject()))
    {
        cJSON *type = cJSON_CreateNumber(CREATE_ROOM);
        cJSON *sender = cJSON_CreateString(username);
        cJSON *receiver = cJSON_CreateString(customer);

        cJSON_AddItemToObject(request_create_room, "type", type);
        cJSON_AddItemToObject(request_create_room, "sender", sender);
        cJSON_AddItemToObject(request_create_room, "receiver", receiver);

        string = cJSON_Print(request_create_room);
        cJSON_Delete(request_create_room);
    }
    return string;
}

char *send_rq_send_msg_client(char *username, gint room_id, char *message, char *date) {
    char *string = NULL;
    cJSON *request_send_msg = NULL;

    if ((request_send_msg = cJSON_CreateObject()))
    {
        cJSON *type = cJSON_CreateNumber(SND_MSG);
        cJSON *id = cJSON_CreateNumber(room_id);
        cJSON *sender = cJSON_CreateString(username);
        cJSON *time_send = cJSON_CreateString(date);
        cJSON *msg = cJSON_CreateString(message);

        cJSON_AddItemToObject(request_send_msg, "type", type);
        cJSON_AddItemToObject(request_send_msg, "room_id", id);
        cJSON_AddItemToObject(request_send_msg, "username", sender);
        cJSON_AddItemToObject(request_send_msg, "time", time_send);
        cJSON_AddItemToObject(request_send_msg, "message", msg);

        string = cJSON_Print(request_send_msg);
        cJSON_Delete(request_send_msg);
    }
    return string;
}

char *send_rq_create_msg_client(int room_id) {
    char *string = NULL;
    cJSON *request_create_msg = NULL;

    if ((request_create_msg = cJSON_CreateObject()))
    {
        cJSON *type = cJSON_CreateNumber(CREATE_MSG);
        cJSON *id = cJSON_CreateNumber(room_id);

        cJSON_AddItemToObject(request_create_msg, "type", type);
        cJSON_AddItemToObject(request_create_msg, "room_id", id);
        string = cJSON_Print(request_create_msg);
        cJSON_Delete(request_create_msg);
    }
    return string;
}

static void receive_rs_create_room_client(cJSON *json, t_chat *chat) {
    cJSON *create_room = cJSON_GetObjectItemCaseSensitive(json, "room_id");  
    //send id  
}

static void receive_rs_create_msg_client(cJSON *json, t_chat *chat) {
    cJSON *create_msg = cJSON_GetObjectItemCaseSensitive(json, "msg_id");  
    //send id  
}

char *send_rq_log_in_client(char *username, char *password) {
    char *string = NULL;
    cJSON *request_log_in = NULL;

    if ((request_log_in = cJSON_CreateObject()))
    {
        cJSON *type = cJSON_CreateNumber(LOGIN);
        cJSON *login = cJSON_CreateString(username);
        cJSON *pass = cJSON_CreateString(password);

        cJSON_AddItemToObject(request_log_in, "type", type);
        cJSON_AddItemToObject(request_log_in, "login", login);
        cJSON_AddItemToObject(request_log_in, "pass", pass);

        string = cJSON_Print(request_log_in);
        printf("created request:\n %s\n", string);
        //request_log_in->string;
        cJSON_Delete(request_log_in);
    }
    return string;
}

char *send_rq_sign_in_client(char *username, char *password) {
    char *string = NULL;
    cJSON *request_sign_up = NULL;

    if ((request_sign_up = cJSON_CreateObject()))
    {
        cJSON *type = cJSON_CreateNumber(SIGNUP);
        cJSON *login = cJSON_CreateString(username);
        cJSON *pass = cJSON_CreateString(password);
        cJSON_AddItemToObject(request_sign_up, "type", type);
        cJSON_AddItemToObject(request_sign_up, "login", login);
        cJSON_AddItemToObject(request_sign_up, "pass", pass);
        string = cJSON_Print(request_sign_up);
        cJSON_Delete(request_sign_up);
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

/*-------------------------------------*/
/*--- Static fuctions definitions ---*/
/*-------------------------------------*/

// static void receive_rq_log_in_server(cJSON *rq, sqlite3 *db) {
//     cJSON *log_in = cJSON_GetObjectItemCaseSensitive(rq, "login");
//     cJSON *pass = cJSON_GetObjectItemCaseSensitive(rq, "pass");

//     // t_rs_status status = sign_up(db, log_in->valuestring, pass->valuestring);
//     /* SERVER PROCESS REQUEST */
//     /* SERVER SEND RESPONCE */
// }

// static char *send_rs_log_in_server(t_rs_status response) {
//     char *string = NULL;
//     cJSON *rs_log_in = NULL;

//     if ((rs_log_in = cJSON_CreateObject()))
//     {
//         cJSON *type = cJSON_CreateNumber(LOGIN);
//         cJSON *status = cJSON_CreateNumber(response);

//         cJSON_AddItemToObject(rs_log_in, "type", type);
//         cJSON_AddItemToObject(rs_log_in, "status", status);

//         string = cJSON_Print(rs_log_in);
//         cJSON_Delete(rs_log_in);
//     }
//     else
//         check_error();
//     return string;
// }