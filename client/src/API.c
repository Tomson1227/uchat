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
        case DELETE_ROOM:

        case DELETE_MSG:
        
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

    /* PROCESS RESPONSE */
    switch((int)status->valuedouble) {
        case LOGIN_OK:
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
    
    /* PROCESS RESPONSE */
    switch((int) status->valuedouble) {
        case SIGNUP_OK:
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

    if ((request_send_msg = cJSON_CreateObject())) {
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
        cJSON_Delete(request_log_in);
    }
    return string;
}

char *send_rq_sign_in_client(char *username, char *password) {
    char *string = NULL;
    cJSON *request_sign_up = NULL;

    if ((request_sign_up = cJSON_CreateObject())) {
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

char *send_rq_search_username(char *start_of_username) {
    char *string = NULL;
    cJSON *req_search_username = NULL;
    if ((req_search_username = cJSON_CreateObject())) {
        cJSON *type = cJSON_CreateNumber(SEARCH_USER);
        cJSON *user = cJSON_CreateString(start_of_username);
        cJSON_AddItemToObject(req_search_username, "type", type);
        cJSON_AddItemToObject(req_search_username, "user", user);
        string = cJSON_Print(req_search_username);
        cJSON_Delete(req_search_username);
    }
    else 
        check_error();
    return string;
}

char *send_rq_delete_room(int room_id) {
    char *string = NULL;
    cJSON *rq_delete_room = NULL;
    if ((rq_delete_room == cJSON_CreateObject())) {
        cJSON *type = cJSON_CreateNumber(DELETE_ROOM);
        cJSON *id = cJSON_CreateNumber(room_id);
        cJSON_AddItemToObject(rq_delete_room, "type", type);
        cJSON_AddItemToObject(rq_delete_room, "id", id);
        string = cJSON_Print(rq_delete_room);
        cJSON_Delete(rq_delete_room);
    } else
        check_error();
    return string; 
}

char *send_rq_delete_msg(int msg_id) {
    char *string = NULL;
    cJSON *rq_delete_msg = NULL;

    if ((rq_delete_msg == cJSON_CreateObject())) {
        cJSON *type = cJSON_CreateNumber(DELETE_MSG);
        cJSON *id = cJSON_CreateNumber(msg_id);
        cJSON_AddItemToObject(rq_delete_msg, "type", type);
        cJSON_AddItemToObject(rq_delete_msg, "id", id);
        string = cJSON_Print(rq_delete_msg);
        cJSON_Delete(rq_delete_msg);
    } else 
        check_error();
    return string;
}

static void check_error(void) {
    const char *error_ptr = cJSON_GetErrorPtr();
    if (error_ptr != NULL)
        fprintf(stderr, "Error before: %s\n", error_ptr);
}
