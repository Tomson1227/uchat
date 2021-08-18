#include "uchat.h"

/*--- Static fuctions declarations ---*/
static void check_error(void);
static void receive_rs_log_in_client(cJSON *rs, t_chat *chat);
static void receive_rs_old_dialogs(cJSON *rq, t_chat *chat);
static void receive_rs_search_username(cJSON *rq, t_chat *chat);  
static void receive_rs_sign_up_client(cJSON *rs, t_chat *chat);
static void receive_rs_create_room_client(cJSON *rs, t_chat *chat);
static void receive_rs_send_msg_client(cJSON *rs, t_chat *chat);
static void receive_old_msgs(cJSON *rs, t_chat *chat);
static void receive_rs_old_dialogs(cJSON *rs, t_chat *chat);

// static void recieve_rq_send_message(cJSON *rq, t_chat *chat);
/*-------------------------------------*/
/*--- Public functions definitions ---*/

void process_rs_client(const char *const string, t_chat *chat) {
    printf("entered the processing response function\n");
    cJSON *rq = NULL;

    if ((rq = cJSON_Parse(string))) {
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
        case READ_MSG:
            receive_old_msgs(rq, chat);
            break;
        case SND_MSG:
            receive_rs_send_msg_client(rq, chat);
            break;
        case SEARCH_USER:
            receive_rs_search_username(rq, chat);  
            break;
        case OLD_DIALOGS:
            receive_rs_old_dialogs(rq, chat);
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

    /* PROCESS RESPONSE */
    switch((int)status->valuedouble) {
        case SUCCESS:
            init_chat_window(chat, 0);
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
        case SUCCESS:
            init_chat_window(chat, 1);
            break;
        case SIGNUP_USER_EXIST:
            display_error_user_exists(chat);
            break;
        case ERROR:
            break;
        default:
            /*UNKNOWN STATUS*/
            break;
    }
}

static void recieve_rq_send_message(cJSON *rq, t_chat *chat) {
    cJSON *username = cJSON_GetObjectItemCaseSensitive(rq, "username");
    cJSON *room_id = cJSON_GetObjectItemCaseSensitive(rq, "room_id");
    cJSON *message = cJSON_GetObjectItemCaseSensitive(rq, "message");
    cJSON *message_id = cJSON_GetObjectItemCaseSensitive(rq, "message_id");
    cJSON *date = cJSON_GetObjectItemCaseSensitive(rq, "date");

    int id_of_room = atoi(room_id->valuestring);
    int id_of_msg = atoi(message_id->valuestring);
    char *sender = cJSON_Print(username);
    char *time = cJSON_Print(date);
    char *msg = cJSON_Print(message);

    recv_message(id_of_room, id_of_msg, time, msg, chat, sender);
}

static void receive_rs_create_room_client(cJSON *json, t_chat *chat) {
    cJSON *id = cJSON_GetObjectItemCaseSensitive(json, "id");
    cJSON *customer = cJSON_GetObjectItemCaseSensitive(json, "customer");
    char *customer_string = cJSON_Print(customer);
    // printf("received customer name: %s\n", customer_string);
    int room_id = atoi(id->valuestring);
    create_dialog(room_id, customer_string, chat);
}

static void receive_rs_old_dialogs(cJSON *rs, t_chat *chat) {
    const cJSON *array_with_dialogs = NULL;
    const cJSON *array_with_msg_id = NULL;
    const cJSON *array_with_room_id = NULL;
    const cJSON *dialog = NULL;
    const cJSON *id = NULL;
    const cJSON *msg_id = NULL;

    array_with_dialogs = cJSON_GetObjectItemCaseSensitive(rs, "dialogs");
    array_with_room_id = cJSON_GetObjectItemCaseSensitive(rs, "id");
    array_with_msg_id = cJSON_GetObjectItemCaseSensitive(rs, "first_message_id");
    int n = cJSON_GetArraySize(array_with_dialogs);
    
    int *ids = NULL;
    int *msg_ids = NULL;
    char **dialogs = NULL;
    ids = malloc(sizeof(int) * n);
    msg_ids = malloc(sizeof(int) * n);
    dialogs = malloc(sizeof(char *) * n);

    for (int i = 0; i < n; i++) {
        dialog = cJSON_GetArrayItem(array_with_dialogs, i);
        dialogs[i] = g_strdup(dialog->valuestring);
    }
    for (int i = 0; i < n; i++) {
        id = cJSON_GetArrayItem(array_with_room_id, i);
        ids[i] = id->valuedouble;
    }
    for (int i = 0; i < n; i++) {
        msg_id = cJSON_GetArrayItem(array_with_dialogs, i);
        msg_ids[i] = msg_id->valuedouble;
    }
    upload_old_dialogs(dialogs, ids, msg_ids, n, chat);
}

static void receive_old_msgs(cJSON *json, t_chat *chat) {
    cJSON *status = cJSON_GetObjectItemCaseSensitive(json, "status");

    if (status->valuedouble == SUCCESS) {
        cJSON *room_id = cJSON_GetObjectItemCaseSensitive(json, "room_id");
        cJSON *message_id = cJSON_GetObjectItemCaseSensitive(json, "message_id");
        cJSON *msg_type = cJSON_GetObjectItemCaseSensitive(json, "msg_type");
        cJSON *message = cJSON_GetObjectItemCaseSensitive(json, "message");
        cJSON *date = cJSON_GetObjectItemCaseSensitive(json, "date");
        cJSON *sender = cJSON_GetObjectItemCaseSensitive(json, "sender");
        
        int id_of_room = atoi(room_id->valuestring);
        int id_of_msg = atoi(message_id->valuestring);
        char *time = cJSON_Print(date);
        char *text = cJSON_Print(message);
        char *username = cJSON_Print(sender); 
        recv_message(id_of_room, id_of_msg, time, text, chat, username);
    }
}

static void receive_rs_send_msg_client(cJSON *json, t_chat *chat) {
    printf("entered receive_rs_send_msg_client function\n");
    cJSON *status = cJSON_GetObjectItemCaseSensitive(json, "status");

    // if (status->valuedouble == 0) {
        cJSON *room_id = cJSON_GetObjectItemCaseSensitive(json, "room_id");
        cJSON *message_id = cJSON_GetObjectItemCaseSensitive(json, "message_id");
        cJSON *date = cJSON_GetObjectItemCaseSensitive(json, "date");

        char *time = g_strdup(date->valuestring);
        printf("got time: %s\n", time);
        int id_of_msg = message_id->valuedouble;
        printf("got id of msg: %d\n", id_of_msg);
        int id_of_room = room_id->valuedouble;
        printf("got id of room: %d\n", id_of_room);
        add_message(id_of_room, id_of_msg, time, chat);
    // }
}

static void receive_rs_search_username(cJSON *json, t_chat *chat) {
    const cJSON *array = NULL;
    const cJSON *name = NULL;
    array = cJSON_GetObjectItemCaseSensitive(json, "user");
    int n = cJSON_GetArraySize(array);

    // if (n == 0) {
    //     char *result = NULL;
    //     name = cJSON_GetObjectItemCaseSensitive(json, "user");
    //     result = malloc(sizeof(char) * strlen(name->valuestring) + 1);
    //     result = g_strdup(name->valuestring);
    //     filter_search(result, 0, chat);   
    // }

    char **output = NULL;
    output = malloc(sizeof(char *) * n);

    for (int i = 0; i < n; i++) {
        name = cJSON_GetArrayItem(array, i);
        output[i] = g_strdup(name->valuestring);
    }
    filter_search(output, n, chat);
}

char *send_req_upload_messages(int type, int room_id) {
    char *string = NULL;
    cJSON *req_old_msgs = NULL;

    if ((req_old_msgs = cJSON_CreateObject())) {
        cJSON *type_of_rq = cJSON_CreateNumber(READ_MSG);
        cJSON *id_of_room = cJSON_CreateNumber(room_id);

        cJSON_AddItemToObject(req_old_msgs, "type", type_of_rq);
        cJSON_AddItemToObject(req_old_msgs, "room_id", id_of_room);

        string = cJSON_Print(req_old_msgs);
        cJSON_Delete(req_old_msgs);
    }
}

char *send_req_old_dialogs(char *username) {
    char *string = NULL;
    cJSON *req_old_dlgs = NULL;

    if ((req_old_dlgs = cJSON_CreateObject()))
    {
        cJSON *type = cJSON_CreateNumber(CREATE_ROOM);
        cJSON *sender = cJSON_CreateString(username);

        cJSON_AddItemToObject(req_old_dlgs, "type", type);
        cJSON_AddItemToObject(req_old_dlgs, "username", sender);

        string = cJSON_Print(req_old_dlgs);
        cJSON_Delete(req_old_dlgs);
    }
    return string;
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

char *send_rq_send_msg_client(char *username, gint room_id, char *message) {
    char *string = NULL;
    cJSON *request_send_msg = NULL;

    if ((request_send_msg = cJSON_CreateObject())) {
        cJSON *type = cJSON_CreateNumber(SND_MSG);
        cJSON *sender = cJSON_CreateString(username);
        cJSON *id = cJSON_CreateNumber(room_id);
        cJSON *msg = cJSON_CreateString(message);
        cJSON *msg_type = cJSON_CreateNumber(MESSAGE);

        cJSON_AddItemToObject(request_send_msg, "type", type);
        cJSON_AddItemToObject(request_send_msg, "username", sender);
        cJSON_AddItemToObject(request_send_msg, "room_id", id);
        cJSON_AddItemToObject(request_send_msg, "message", msg);
        cJSON_AddItemToObject(request_send_msg, "msg_type", msg_type);

        string = cJSON_Print(request_send_msg);
        cJSON_Delete(request_send_msg);
    }
    return string;
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
