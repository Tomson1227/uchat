#include "uchat_server.h"

/*--- Static fuctions declarations ---*/
static char *server_upload_old_dialogs(cJSON *rq); 
static char *server_delete_message(cJSON *rq); //In develop
static char *server_send_message(cJSON *rq);
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

        switch((int)type->valueint) {
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
                ReadMessage(rq, fd);
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

void send_response(char* message, int fd)
{
    if(message && send(fd, message, strlen(message), 0) < 0)
        perror("send fail");
}

cJSON *create_response(t_message *message)
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

/*-------------------------------------*/
/*--- Static fuctions definitions ---*/
/*-------------------------------------*/

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

        for (register unsigned int index = 0; message.Data.search_user.users[index]; ++index) {
            user = cJSON_CreateString(message.Data.search_user.users[index]);
            cJSON_AddItemToArray(users, user);
            free(message.Data.search_user.users[index]);
        }

        cJSON_AddItemToObject(response, "user", users);

        line = cJSON_Print(response);
        cJSON_Delete(response);
        free(message.Data.search_user.users);
    }

    return line;
}

static char *server_send_message(cJSON *rq) 
{
    t_message message;
    char *line = NULL;
    cJSON *response = NULL;
    cJSON *roomID = cJSON_GetObjectItemCaseSensitive(rq, "room_id");
    cJSON *username = cJSON_GetObjectItemCaseSensitive(rq, "username");
    cJSON *msg = cJSON_GetObjectItemCaseSensitive(rq, "message");
    cJSON *message_type = cJSON_GetObjectItemCaseSensitive(rq, "msg_type");

    SendMessage(&message, username->valuestring, roomID->valueint, msg->valuestring, (t_msg_type) message_type->valueint);

    if((response = create_response(&message))) {
        cJSON *roomID = cJSON_CreateNumber(message.Data.create_message.room_id);
        cJSON *messageID = cJSON_CreateNumber(message.Data.create_message.message_id);
        cJSON *date = cJSON_CreateString(message.Data.create_message.date);

        cJSON_AddItemToObject(response, "room_id", roomID);
        cJSON_AddItemToObject(response, "message_id", messageID);
        cJSON_AddItemToObject(response, "date", date);

        line = cJSON_Print(response);
        cJSON_Delete(response);
    }

    return line;
}

static char *server_create_room(cJSON *rq) 
{
    t_message message;
    char *line = NULL;
    cJSON *response = NULL;
    cJSON *username = cJSON_GetObjectItemCaseSensitive(rq, "sender");
    cJSON *customer = cJSON_GetObjectItemCaseSensitive(rq, "receiver");

    CreateRoom(&message, username->valuestring, customer->valuestring);

    if((response = create_response(&message))) {
        cJSON *ID = cJSON_CreateNumber(message.Data.create_room.id);
        cJSON *customer = cJSON_CreateString(message.Data.create_room.customer);

        cJSON_AddItemToObject(response, "id", ID);
        cJSON_AddItemToObject(response, "customer", customer);

        line = cJSON_Print(response);
        cJSON_Delete(response);
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

    /* DeleteRoom function from DB*/
    DeleteRoom(&message, roomID->valueint);
    
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

    DeleteMessage(&message, roomID->valueint);
    
    if((response = create_response(&message))) {
        cJSON *ID = cJSON_CreateNumber(message.Data.delete_message.id);

        cJSON_AddItemToObject(response, "id", ID);

        line = cJSON_Print(response);
        cJSON_Delete(response);
    }
    printf("formed a line: %s\n", line);
    return line;
}

static char *server_edit_message(cJSON *rq) 
{
    t_message message;
    char *line = NULL;
    cJSON *response = NULL;
    cJSON *roomID = cJSON_GetObjectItemCaseSensitive(rq, "id");
    cJSON *newMessage = cJSON_GetObjectItemCaseSensitive(rq, "new_msg");
    
    EditMessage(&message, roomID->valueint, newMessage->valuestring);
    
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
    
    UploadOldDialogs(&message, username->valuestring);
    
    if((response = create_response(&message))) {
        cJSON *dialogs = cJSON_CreateArray();
        cJSON *room_ids = cJSON_CreateArray();
        cJSON *id = NULL;
        cJSON *dialog = NULL;

        for (register unsigned int index = 0; message.Data.upload_old_dialogs.dialogs[index]; ++index) {
            dialog = cJSON_CreateString(message.Data.upload_old_dialogs.dialogs[index]);
            id = cJSON_CreateNumber(message.Data.upload_old_dialogs.id[index]);
            cJSON_AddItemToArray(dialogs, dialog);
            cJSON_AddItemToArray(room_ids, id);
            free(message.Data.upload_old_dialogs.dialogs[index]);
        }

        cJSON_AddItemToObject(response, "dialogs", dialogs);
        cJSON_AddItemToObject(response, "id", room_ids);

        line = cJSON_Print(response);
        cJSON_Delete(response);
        free(message.Data.upload_old_dialogs.dialogs);
        free(message.Data.upload_old_dialogs.id);
    }
    
    return line;
}

static void check_error(void) {
    const char *error_ptr = cJSON_GetErrorPtr();

    if (error_ptr != NULL)
        fprintf(stderr, "Error before: %s\n", error_ptr);
}
