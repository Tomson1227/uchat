#include "uchat_server.h"

/*--- Static fuctions declarations ---*/
static cJSON *create_response(t_message *message);
static void send_response(char* message, int fd);
// static char *server_user_search(cJSON *rq);
static char *server_create_room(cJSON *rq); 
static char *server_create_msg(cJSON *rq);
static char *server_sign_up(cJSON *rq); 
static char *server_log_in(cJSON *rq); 
static void check_error(void);
/*-------------------------------------*/
/*--- Public functions definitions ---*/

void process_rq_server(const char *const string, sqlite3 *db, int fd) {
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
                response = server_create_msg(rq);
                break;
            case READ_MSG:
                break;
            case SEARCH_USER:
                // response = server_user_search(rq);
                break;
            case DELETE_ROOM:
                break;
            case DELETE_MSG:
                break;
            case EDIT_MSG:
                break;
            case OLD_DIALOGS:
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

// static char *server_user_search(cJSON *rq) 
// {
//     t_message message;
//     char *line = NULL;
//     cJSON *response = NULL;
//     cJSON *username = cJSON_GetObjectItemCaseSensitive(rq, "user");

//     UserSearch(&message, username->valuestring);

//     if((response = create_response(&message))) {
//         cJSON *user = cJSON_CreateStringArray(message.Data.search_user.user, message.Data.search_user.count);
//         cJSON_AddItemToObject(response, "user", user);

//         line = cJSON_Print(response);
//         cJSON_Delete(response);
//     }

//     return line;
// }

static char *server_create_msg(cJSON *rq) 
{
    t_message message;
    char *line = NULL;
    cJSON *response = NULL;
    cJSON *roomID = cJSON_GetObjectItemCaseSensitive(rq, "username");

    CreateMessage(&message, roomID->valuedouble);

    if((response = create_response(&message))) {
        cJSON *roomID = cJSON_CreateNumber(message.Data.create_msg.room_id);
        cJSON *messageID = cJSON_CreateNumber(message.Data.create_msg.msg_id);
        cJSON *date = cJSON_CreateString(message.Data.create_msg.date);
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

static void check_error(void) {
    const char *error_ptr = cJSON_GetErrorPtr();

    if (error_ptr != NULL)
        fprintf(stderr, "Error before: %s\n", error_ptr);
}
