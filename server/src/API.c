#include "uchat_server.h"

/*--- Static fuctions declarations ---*/
static cJSON *create_response(t_message *message);
static void send_response(char* message, int fd);
static char *server_upload_old_dialogs(cJSON *rq); 
static char *server_delete_message(cJSON *rq); //In develop
static char *server_send_message(cJSON *rq);
static char *server_read_message(cJSON *rq); //In develop
static char *server_edit_message(cJSON *rq); //In develop
static char *server_create_room(cJSON *rq); 
static char *server_delete_room(cJSON *rq); //In develop
static char *server_user_search(cJSON *rq); //In develop
static char *server_sign_up(cJSON *rq); 
static char *server_log_in(cJSON *rq); 
static void check_error(void);
/*-------------------------------------*/
/*--- Public functions definitions ---*/

void process_rq_server(const char *const string, int fd)
{
    cJSON *rq = NULL;
    char *response = NULL;

    if ((rq = cJSON_Parse(string))) {
        cJSON *type = cJSON_GetObjectItemCaseSensitive(rq, "type");

        switch((int)type->valuedouble) {
            case LOGIN: 
                response = server_log_in(rq);
                break;
            case SIGNUP:
                response = server_sign_up(rq);
                break;
            case CREATE_ROOM:
                response = server_create_room(rq);
                break;
            case SND_MSG:
                response = server_send_message(rq);
                break;
            case READ_MSG:
                response = server_read_message(rq);
                break;
            case SEARCH_USER:
                response = server_user_search(rq);
                break;
            case DELETE_ROOM:
                response = server_delete_room(rq);
                break;
            case DELETE_MSG:
                response = server_delete_message(rq);
                break;
            case EDIT_MSG:
                response = server_edit_message(rq);
                break;
            case OLD_DIALOGS:
                response = server_upload_old_dialogs(rq);
                break;
            default:
                /* UNKNOWN REQUEST */
                break;
        }

        send_response(response, fd);
        cJSON_Delete(rq);

        free(response);
    }
    else
        check_error();
}

/*-------------------------------------*/
/*--- Static fuctions definitions ---*/
/*-------------------------------------*/

static cJSON *create_response(t_message *message)
{
    cJSON *json = NULL;

    if ((json = cJSON_CreateObject())) {
        cJSON *type = cJSON_CreateNumber(message->API);
        cJSON *status = cJSON_CreateNumber(message->status);

        cJSON_AddItemToObject(json, "type", type);
        cJSON_AddItemToObject(json, "status", status);
    }

    return json;
}

static void send_response(char* message, int fd)
{
    if(message && send(fd, message, strlen(message), 0) < 0)
        perror("send fail");
}

static char *server_user_search(cJSON *rq) 
{
    t_message message;
    char *line = NULL;
    cJSON *response = NULL;
    cJSON *username = cJSON_GetObjectItemCaseSensitive(rq, "user");

    UserSearch(&message, username->valuestring);

    if((response = create_response(&message))) {
        cJSON *users = cJSON_CreateArray();
        cJSON *user = NULL;

        for (register unsigned int index = 0; message.Data.search_user.user[index]; ++index) {
            user = cJSON_CreateString(message.Data.search_user.user[index]);
            cJSON_AddItemToArray(users, user);
            free(message.Data.search_user.user[index]);
        }

        cJSON_AddItemToObject(response, "user", user);

        line = cJSON_Print(response);
        cJSON_Delete(response);
        free(message.Data.search_user.user);
    }

    return line;
}

static char *server_read_message(cJSON *rq)
{
    t_message message;
    char *line = NULL;
    cJSON *response = NULL;
    cJSON *roomID = cJSON_GetObjectItemCaseSensitive(rq, "room_id");

    /* ReadMessage function from DB */
    // ReadMessage(&message, roomID->valuedouble);

    if((response = create_response(&message))) {
        cJSON *roomID = cJSON_CreateNumber(message.Data.read_message.room_id);
        cJSON *messageID = cJSON_CreateNumber(message.Data.read_message.message_id);
        cJSON *messageType = cJSON_CreateNumber(message.Data.read_message.message_type);
        cJSON *msg = cJSON_CreateString(message.Data.read_message.message);
        cJSON *sender = cJSON_CreateString(message.Data.read_message.sender);
        cJSON *date = cJSON_CreateString(message.Data.read_message.date);
        cJSON *update = cJSON_CreateNumber(message.Data.read_message.update);

        cJSON_AddItemToObject(response, "room_id", roomID);
        cJSON_AddItemToObject(response, "message_id", messageID);
        cJSON_AddItemToObject(response, "msg_type", messageType);
        cJSON_AddItemToObject(response, "message", msg);
        cJSON_AddItemToObject(response, "date", date);
        cJSON_AddItemToObject(response, "sender", sender);
        cJSON_AddItemToObject(response, "update", update);

        line = cJSON_Print(response);
        cJSON_Delete(response);
        free(message.Data.read_message.date);
        free(message.Data.read_message.sender);
        free(message.Data.read_message.message);
    }

    return line;
} 

static char *server_send_message(cJSON *rq) 
{
    t_message message;
    char *line = NULL;
    cJSON *response = NULL;
    cJSON *roomID = cJSON_GetObjectItemCaseSensitive(rq, "username");

    SendMessage(&message, roomID->valuedouble);

    if((response = create_response(&message))) {
        cJSON *roomID = cJSON_CreateNumber(message.Data.create_message.room_id);
        cJSON *messageID = cJSON_CreateNumber(message.Data.create_message.message_id);
        cJSON *date = cJSON_CreateString(message.Data.create_message.date);

        cJSON_AddItemToObject(response, "room_id", roomID);
        cJSON_AddItemToObject(response, "message_id", messageID);
        cJSON_AddItemToObject(response, "date", date);

        line = cJSON_Print(response);
        cJSON_Delete(response);
        free(message.Data.create_message.date);
    }

    return line;
}

static char *server_create_room(cJSON *rq) 
{
    t_message message;
    char *line = NULL;
    cJSON *response = NULL;
    cJSON *username = cJSON_GetObjectItemCaseSensitive(rq, "username");
    cJSON *customer = cJSON_GetObjectItemCaseSensitive(rq, "customer");

    CreateRoom(&message, username->valuestring, customer->valuestring);

    if((response = create_response(&message))) {
        cJSON *ID = cJSON_CreateNumber(message.Data.create_room.id);
        cJSON *customer = cJSON_CreateString(message.Data.create_room.customer);

        cJSON_AddItemToObject(response, "id", ID);
        cJSON_AddItemToObject(response, "customer", customer);

        line = cJSON_Print(response);
        cJSON_Delete(response);
        free(message.Data.create_room.customer);
    }
    
    return line;
}

static char *server_log_in(cJSON *rq) 
{    
    t_message message;
    char *line = NULL;
    cJSON *response = NULL;
    cJSON *log_in = cJSON_GetObjectItemCaseSensitive(rq, "login");
    cJSON *pass = cJSON_GetObjectItemCaseSensitive(rq, "pass");
    
    LogIn(&message, log_in->valuestring, pass->valuestring);
    
    if((response = create_response(&message))) {
        line = cJSON_Print(response);
        cJSON_Delete(response);
    }
    
    return line;
}

static char *server_sign_up(cJSON *rq) 
{
    t_message message;
    char *line = NULL;
    cJSON *response = NULL;
    cJSON *log_in = cJSON_GetObjectItemCaseSensitive(rq, "login");
    cJSON *pass = cJSON_GetObjectItemCaseSensitive(rq, "pass");

    SignUp(&message, log_in->valuestring, pass->valuestring);
    
    if((response = create_response(&message))) {
        line = cJSON_Print(response);
        cJSON_Delete(response);
    }
    
    return line;
}

static char *server_delete_room(cJSON *rq) 
{
    t_message message;
    char *line = NULL;
    cJSON *response = NULL;
    cJSON *roomID = cJSON_GetObjectItemCaseSensitive(rq, "id");

    /* DeleteRoom function from DB */
    // DeleteRoom(&message, roomID->valuedouble);
    
    if((response = create_response(&message))) {
        cJSON *ID = cJSON_CreateNumber(message.Data.delete_room.id);

        cJSON_AddItemToObject(response, "id", ID);

        line = cJSON_Print(response);
        cJSON_Delete(response);
    }
    
    return line;
}

static char *server_delete_message(cJSON *rq) 
{
    t_message message;
    char *line = NULL;
    cJSON *response = NULL;
    cJSON *roomID = cJSON_GetObjectItemCaseSensitive(rq, "id");

    /* DeleteMessage function from DB */
    // DeleteMessage(&message, roomID->valuedouble);
    
    if((response = create_response(&message))) {
        cJSON *ID = cJSON_CreateNumber(message.Data.delete_message.id);

        cJSON_AddItemToObject(response, "id", ID);

        line = cJSON_Print(response);
        cJSON_Delete(response);
    }
    
    return line;
}

static char *server_edit_message(cJSON *rq) 
{
    t_message message;
    char *line = NULL;
    cJSON *response = NULL;
    cJSON *roomID = cJSON_GetObjectItemCaseSensitive(rq, "id");
    cJSON *newMessage = cJSON_GetObjectItemCaseSensitive(rq, "new_msg");
    
    /* EditMessage function from DB */
    // EditMessage(&message, roomID->valuedouble, newMessage->valuestring);
    
    if((response = create_response(&message))) {
        line = cJSON_Print(response);
        cJSON_Delete(response);
    }
    
    return line;
}

static char *server_upload_old_dialogs(cJSON *rq) 
{
    t_message message;
    char *line = NULL;
    cJSON *response = NULL;
    cJSON *username = cJSON_GetObjectItemCaseSensitive(rq, "username");
    
    /* UploadOldDialogs function from DB */
    // UploadOldDialogs(&message, username->valuestring);
    
    if((response = create_response(&message))) {
        cJSON *users = cJSON_CreateArray();
        cJSON *user = NULL;

        for (register unsigned int index = 0; ; ++index) {
            user = cJSON_CreateString(message.Data.search_user.user[index]);
            cJSON_AddItemToArray(users, user);
            free(message.Data.search_user.user[index]);
        }

        cJSON_AddItemToObject(response, "user", user);

        line = cJSON_Print(response);
        cJSON_Delete(response);
        free(message.Data.search_user.user);
    }
    
    return line;
}

static void check_error(void) {
    const char *error_ptr = cJSON_GetErrorPtr();

    if (error_ptr != NULL)
        fprintf(stderr, "Error before: %s\n", error_ptr);
}
