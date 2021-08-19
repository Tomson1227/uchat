#include "uchat_server.h"

void createmessage(t_message *message, int roomID);

static sqlite3 *db = NULL;
static int callback(void *data, int argc, char **argv, char **azColName);
static int checkLogin(char *user_login); 
static int getUserID(char *username);
static int countSelectedRows(int ID, int sw);
static char *getUserName(int userID);

static char query[200];

/* Need to be developed */
void ReadMessage(cJSON *rq, int fd)
{
    t_message message;
    message.API = READ_MSG;
    message.status = SUCCESS;

    cJSON *response = NULL;
    cJSON *roomID = cJSON_GetObjectItemCaseSensitive(rq, "room_id");
    
    int length, size, count = 0, senderID;
    sqlite3_stmt *stmt;

    size = countSelectedRows(roomID->valuedouble, 1);

    sprintf(query, "SELECT * FROM MSSGS WHERE ROOM_ID = %d", (int) roomID->valuedouble);
    sqlite3_prepare_v2(db, query, -1, &stmt, NULL);

    while (count < size) {
        sqlite3_step(stmt);

        if((response = create_response(&message))) {
            cJSON *roomID = cJSON_CreateNumber(sqlite3_column_int(stmt, 2));
            cJSON *messageID = cJSON_CreateNumber(sqlite3_column_int(stmt, 0));
            cJSON *messageType = cJSON_CreateNumber(M_MESSAGE);
            cJSON *msg = cJSON_CreateString(sqlite3_column_text(stmt, 4));
            cJSON *sender = cJSON_CreateString(getUserName(sqlite3_column_int(stmt, 1)));
            cJSON *date = cJSON_CreateString(sqlite3_column_text(stmt, 3));
            cJSON *update = cJSON_CreateNumber(true);

            cJSON_AddItemToObject(response, "room_id", roomID);
            cJSON_AddItemToObject(response, "message_id", messageID);
            cJSON_AddItemToObject(response, "msg_type", messageType);
            cJSON_AddItemToObject(response, "message", msg);
            cJSON_AddItemToObject(response, "date", date);
            cJSON_AddItemToObject(response, "sender", sender);
            cJSON_AddItemToObject(response, "update", update);

            send_response(response->valuestring, fd);
            cJSON_Delete(response);
        }

        ++count;
    }


    return;
}

void DeleteRoom(t_message *message, int roomID)
{
    int length, rc;
    char *errMssg;

    message->API = DELETE_ROOM;
    message->status = SUCCESS;

    sprintf(query, "DELETE FROM ROOMS WHERE ID = %d", roomID);

    rc = sqlite3_exec(db, query, 0, 0, &errMssg);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL Error: %s\n", errMssg);
        sqlite3_free(errMssg);
        message->status = ERROR;
    } else
        message->Data.delete_room.id = roomID;
}

void DeleteMessage(t_message *message, int messageID)
{
    message->API = DELETE_MSG;
    message->status = SUCCESS;

    int length, rc;
    char *errMssg;

    sprintf(query, "DELETE FROM MSSGS WHERE ID = %d", messageID);

    rc = sqlite3_exec(db, query, 0, 0, &errMssg);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", errMssg);
        sqlite3_free(errMssg);
        message->status = ERROR;
    } else
        message->Data.delete_message.id = messageID;
}

void EditMessage(t_message *message, int messageID, char *newMessage)
{
    message->API = EDIT_MSG;
    message->status = SUCCESS;

    int length, rc;
    char *errMssg;

    sprintf(query, "UPDATE MSSGS SET message = '%s' WHERE ID = '%d'", newMessage, messageID);

    rc = sqlite3_exec(db, query, 0, 0, &errMssg);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL Error: %s\n", errMssg);
        sqlite3_free(errMssg);
        message->status = ERROR;
    } else
        message->Data.edit_message.id = messageID;
}

void UploadOldDialogs(t_message *message, char *username)
{
    sqlite3_stmt *stmt;
    int length, rc, count, userID, roomID, size;
    char *errMssg, *roomName;

    message->API = OLD_DIALOGS;
    message->status = SUCCESS;

    userID = getUserID(username);
    size = countSelectedRows(userID, 0);
    
    message->Data.upload_old_dialogs.dialogs = (char **)calloc(size + 1, sizeof(char *));
    message->Data.upload_old_dialogs.id = (int *)calloc(size, sizeof(int));

    sprintf(query, "SELECT ID, NAME FROM ROOMS WHERE USER_ID = %d", userID);
    sqlite3_prepare_v2(db, query, -1, &stmt, NULL);
    
    for (count = 0; count < size; ++count) {
        sqlite3_step(stmt);
        message->Data.upload_old_dialogs.id[count] = sqlite3_column_int(stmt, 0);
        message->Data.upload_old_dialogs.dialogs[count] = strdup(sqlite3_column_text(stmt, 1));
    }

    message->Data.upload_old_dialogs.dialogs[count] = NULL;
}

void SendMessage(t_message *message, char *username, int roomID, char *text, t_msg_type M_MESSAGE)
{
    message->API = SND_MSG;
    message->status = SUCCESS;
    message->Data.create_message.room_id = roomID;
    
    int length, rc, userID;
    char *errMsg, Time[50];
    char *selectID = "SELECT MAX(ID) FROM MSSGS";
    sqlite3_stmt *stmt;
    time_t currTime;
    struct tm *local = localtime(&currTime);

    userID = getUserID(username);

    strftime(Time, sizeof(Time), "%a %b %d %r", local);
    memcpy(message->Data.create_message.date, Time, strlen(Time));

    sprintf(query,
        "INSERT INTO MSSGS (ROOM_ID, USER_ID, message, date) VALUES (%d, %d, '%s', '%s')", 
        roomID, userID, text, Time);

    rc = sqlite3_exec(db, query, 0, 0, &errMsg);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", errMsg);
        sqlite3_free(errMsg);
        message->status = ERROR;
    } else {
        sqlite3_prepare_v2(db, selectID, -1, &stmt, NULL);
        sqlite3_step(stmt);

        message->Data.create_message.message_id = sqlite3_column_int(stmt, 0);
    }
}

void CreateRoom(t_message *message, char *user, char *customer) 
{
    int length, userID, customerID, rc, ID;
    char *roomIDQuery = "SELECT MAX(ID) FROM ROOMS";
    char *errMsg = NULL;
    sqlite3_stmt *stmt;

    message->API = CREATE_ROOM;
    message->status = SUCCESS;
    memcpy(message->Data.create_room.customer, customer, strlen(customer));

    userID = getUserID(user);
    customerID = getUserID(customer);

    sprintf(query, "INSERT INTO ROOMS (USER_ID, CUSTOMER_ID, NAME) VALUES (%d, %d, '%s')", userID, customerID, customer);

    rc = sqlite3_exec(db, query, 0, 0, &errMsg);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", errMsg);
        sqlite3_free(errMsg);
        message->status = ERROR;
    } else {
        sqlite3_prepare_v2(db, roomIDQuery, -1, &stmt, NULL);
        sqlite3_step(stmt);
        message->Data.create_room.id = sqlite3_column_int(stmt, 0);
    }
}

void UserSearch(t_message *message, char *searchText) 
{
    message->API = SEARCH_USER;
    message->status = SUCCESS;

    int lengthSearch, count = 0, size = 1;
    const unsigned char *result;
    sqlite3_stmt *stmt;

    message->Data.search_user.users = calloc(size, sizeof(char *));

    sprintf(query, "SELECT LOGIN FROM USRS WHERE LOGIN LIKE '%c%s%c';", '%', searchText, '%');
    sqlite3_prepare_v2(db, query, -1, &stmt, NULL);

    while (1) {
        sqlite3_step(stmt);
        result = sqlite3_column_text(stmt, 0);

        if (result != NULL) {
            message->Data.search_user.users[count] = strdup(result);
            message->Data.search_user.users = realloc(message->Data.search_user.users, ++size * sizeof(char *));
        } else {
            message->Data.search_user.users[count] = NULL;
            break;
        }

        ++count;
    }
}

void SignUp(t_message *message, char *user_login, char *user_pass)
{
    message->API = SIGNUP;
    message->status = SUCCESS;
    sqlite3_stmt *stmt;
    char *zErrMsg = NULL;
    int lengthSignUp, ID, rc;

    if (checkLogin(user_login)) {
        message->status = SIGNUP_USER_EXIST;
        return;
    }

    char *idQuery = "SELECT MAX(ID) FROM USRS";
    sqlite3_prepare_v2(db, idQuery, -1, &stmt, NULL);
    sqlite3_step(stmt);

    ID = sqlite3_column_int(stmt, 0) + 1;

    sprintf(query, "INSERT INTO USRS (ID, LOGIN, PASS) VALUES (%d, '%s', '%s');", ID, user_login, user_pass);

    rc = sqlite3_exec(db, query, callback, 0, &zErrMsg);

    if( rc != SQLITE_OK ) {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
        message->status = ERROR;
    }
}

void LogIn(t_message *message, char *user_login, char *user_pass)
{
    message->API = LOGIN;
    message->status = SUCCESS;
    sqlite3_stmt *stmt;
    const unsigned char *pass;

    if (!checkLogin(user_login)) {
        message->status = LOGIN_WRONG_USER;
        return;
    }
    
    sprintf(query, "SELECT PASS FROM USRS WHERE LOGIN  = '%s';", user_login);

    sqlite3_prepare_v2(db, query, -1, &stmt, NULL);
    sqlite3_step(stmt);

    pass = sqlite3_column_text(stmt, 0);

    if(strcmp((const char*)pass, user_pass))
        message->status = LOGIN_WRONG_PASS;
}

void Init_DB(t_server *server)
{
    char *zErrMsg = 0;
    char *sql;
    int rc = sqlite3_open("uchat.db", &db);
    server->db = db;

    if( rc ) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(server->db));
        return;
    } else {
        fprintf(stderr, "Opened database successfully\n");
    }

    sql = "CREATE TABLE IF NOT EXISTS USRS(" \
    "ID INT PRIMARY KEY," \
    "LOGIN TEXT," \
    "PASS  TEXT);";

    // sql = "SELECT * FROM USRS";

    rc = sqlite3_exec(server->db, sql, callback, 0, &zErrMsg);

    if( rc != SQLITE_OK ){
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    } else {
        fprintf(stdout, "Users table created successfully\n");
    }

    sql = "CREATE TABLE IF NOT EXISTS ROOMS(ID INTEGER PRIMARY KEY NOT NULL, NAME TEXT, USER_ID INT, CUSTOMER_ID INT);";

//    sql = "DROP TABLE ROOMS";

//    sql = "SELECT * FROM ROOMS";

    rc = sqlite3_exec(server->db, sql, callback, 0, &zErrMsg);

    if( rc != SQLITE_OK ){
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    } else {
        fprintf(stdout, "Rooms table created successfully\n");
    }

    sql = "CREATE TABLE IF NOT EXISTS MSSGS(" \
    "ID INTEGER PRIMARY KEY NOT NULL," \
    "USER_ID INT," \
    "ROOM_ID INT," \
    "DATE INT," \
    "message TEXT);";

    // sql = "SELECT * FROM MSSGS";

    rc = sqlite3_exec(server->db, sql, callback, 0, &zErrMsg);

    if( rc != SQLITE_OK ){
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    } else {
        fprintf(stdout, "Messages table created successfully\n");
    }
}

static int countSelectedRows(int ID, int sw) {
    int count;
    static char countQuery[55];
    sqlite3_stmt *stmt;

    switch (sw) {
        case 0:
            sprintf(countQuery, "SELECT COUNT(*) FROM ROOMS WHERE USER_ID = %d", ID);
            break;
        
        case 1:
            sprintf(countQuery, "SELECT COUNT(*) FROM MSSGS WHERE ROOM_ID = %d", ID);
            break;
        
        default:
            break;
    }

    sqlite3_prepare_v2(db, countQuery, -1, &stmt, NULL);
    sqlite3_step(stmt);

    count = sqlite3_column_int(stmt, 0);

    return count;
}

static int callback(void *data, int argc, char **argv, char **azColName)
{
    int i;
    fprintf(stderr, "%s: ", (const char*)data);

    for(i = 0; i<argc; i++){
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }

    printf("\n");
    return 0;
}

static int getUserID(char *username)
{
    int length, id;
    static char idQuery[60];
    sqlite3_stmt *stmt;

    sprintf(idQuery, "SELECT ID FROM USRS WHERE LOGIN = '%s'", username);

    sqlite3_prepare_v2(db, idQuery, -1, &stmt, NULL);
    sqlite3_step(stmt);

    id = sqlite3_column_int(stmt, 0);

    sqlite3_finalize(stmt);

    return id;
}

static int checkLogin(char *user_login)
{
    const unsigned char *checkLogin;
    static char checkQuery[60];
    sqlite3_stmt *stmt;

    sprintf(checkQuery, "SELECT LOGIN FROM USRS WHERE LOGIN = '%s';", user_login);

    sqlite3_prepare_v2(db, checkQuery, -1, &stmt, NULL);
    sqlite3_step(stmt);

    checkLogin = sqlite3_column_text(stmt, 0);

    return !!checkLogin;
}

static char *getUserName(int userID) {
    sqlite3_stmt *stmt;
    int length;
    static char getNameQuery[50];
    char *userName;

    sprintf(getNameQuery, "SELECT LOGIN FROM USRS WHERE ID = %d", userID);

    sqlite3_prepare_v2(db, getNameQuery, -1, &stmt, NULL);
    sqlite3_step(stmt);

    userName = (char *)sqlite3_column_text(stmt, 0);

    return userName;
}
