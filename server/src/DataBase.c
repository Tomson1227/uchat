#include "uchat_server.h"

void createmessage(t_message *message, int roomID);

static sqlite3 *db = NULL;
static int callback(void *data, int argc, char **argv, char **azColName);
static int checkLogin(char *user_login); 
static int getUserID(char *username);
static int countSelectedRows(int userID);

/* Need to be developed */
void ReadMessage(t_message *message, int roomID)
{
    message->API = READ_MSG;
    message->status = SUCCESS;

    // message->Data.read_message.room_id;
    // message->Data.read_message.message_id;
    // message->Data.read_message.message_type;
    // message->Data.read_message.update;
    // message->Data.read_message.message;
    // message->Data.read_message.sender;
    // message->Data.read_message.date;
}

void DeleteRoom(t_message *message, int roomID)
{
    int length, rc;
    char *deleteRoomQuery, *errMssg;

    message->API = DELETE_ROOM;
    message->status = SUCCESS;

    length = snprintf(NULL, 0, "DELETE FROM ROOMS WHERE ID = %d", roomID);

    if (!(deleteRoomQuery = (char *)calloc(length, sizeof(char)))) {
        perror("Allocation fail!\n");
        message->status = ERROR;
        return;
    }
    
    sprintf(deleteRoomQuery, "DELETE FROM ROOMS WHERE ID = %d", roomID);
    rc = sqlite3_exec(db, deleteRoomQuery, 0, 0, &errMssg);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL Error: %s\n", errMssg);
        sqlite3_free(errMssg);
        message->status = ERROR;
    } else
        message->Data.delete_room.id = roomID;

    free(deleteRoomQuery);
}

void DeleteMessage(t_message *message, int messageID)
{
    message->API = DELETE_MSG;
    message->status = SUCCESS;

    int length, rc;
    char *deleteMessageQuery, *errMssg;

    length = snprintf(NULL, 0, "DELETE FROM MSSGS WHERE ID = %d", messageID);

    if (!(deleteMessageQuery = (char *)calloc(length, sizeof(char)))) {
        perror("Allocation fail!\n");
        message->status = ERROR;
        return;
    }

    sprintf(deleteMessageQuery, "DELETE FROM MSSGS WHERE ID = %d", messageID);
    rc = sqlite3_exec(db, deleteMessageQuery, 0, 0, &errMssg);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", errMssg);
        sqlite3_free(errMssg);
        message->status = ERROR;
    } else
        message->Data.delete_message.id = messageID;

    free(deleteMessageQuery);
}

void EditMessage(t_message *message, int messageID, char *newMessage)
{
    message->API = EDIT_MSG;
    message->status = SUCCESS;

    int length, rc;
    char *editMessageQuery, *errMssg;

    length = snprintf(NULL, 0, "UPDATE MSSGS SET message = '%s' WHERE ID = '%d'", newMessage, messageID);

    if (!(editMessageQuery = (char *)calloc(length, sizeof(char)))) {
        perror("Allocation fail!\n");
        message->status = ERROR;
        return;
    }

    sprintf(editMessageQuery, "UPDATE MSSGS SET message = '%s' WHERE ID = '%d'", newMessage, messageID);
    rc = sqlite3_exec(db, editMessageQuery, 0, 0, &errMssg);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL Error: %s\n", errMssg);
        sqlite3_free(errMssg);
        message->status = ERROR;
    } else
        message->Data.edit_message.id = messageID;

    free(editMessageQuery);
}

void UploadOldDialogs(t_message *message, char *username)
{
    sqlite3_stmt *stmt;
    int length, rc, count, userID, roomID, size;
    char *uploadDialogsQuery, *errMssg, *roomName;

    message->API = OLD_DIALOGS;
    message->status = SUCCESS;

    userID = getUserID(username);
    size = countSelectedRows(userID);
    
    message->Data.upload_old_dialogs.dialogs = (char **)calloc(size + 1, sizeof(char *));
    message->Data.upload_old_dialogs.id = (int *)calloc(size, sizeof(int));

    length = snprintf(NULL, 0, "SELECT ID, NAME FROM ROOMS WHERE USER_ID = %d", userID);

    if (!(uploadDialogsQuery = (char *)calloc(length, sizeof(char)))) {
        perror("Allocation fali!\n");
        message->status = ERROR;
        return;
    }

    sprintf(uploadDialogsQuery, "SELECT ID, NAME FROM ROOMS WHERE USER_ID = %d", userID);
    sqlite3_prepare_v2(db, uploadDialogsQuery, -1, &stmt, NULL);
    
    for (count = 0; count < size; ++count) {
        sqlite3_step(stmt);
        message->Data.upload_old_dialogs.id[count] = sqlite3_column_int(stmt, 0);
        message->Data.upload_old_dialogs.dialogs[count] = strdup(sqlite3_column_text(stmt, 1));
        // printf("%s %d\n", message->Data.upload_old_dialogs.dialogs[count], message->Data.upload_old_dialogs.id[count]);
    }

    message->Data.upload_old_dialogs.dialogs[count] = NULL;
    // printf("%s\n", message->Data.upload_old_dialogs.dialogs[count]);
    
    free(uploadDialogsQuery);
}

void SendMessage(t_message *message, char *username, int roomID, char *text, t_msg_type M_MESSAGE)
{
    message->API = SND_MSG;
    message->status = SUCCESS;
    message->Data.create_message.room_id = roomID;
    
    int length, rc, userID;
    char *createmessageQuery, *errMsg, Time[50];
    char *selectID = "SELECT MAX(ID) FROM MSSGS";
    sqlite3_stmt *stmt;
    time_t currTime;
    struct tm *local = localtime(&currTime);

    userID = getUserID(username);

    strftime(Time, sizeof(Time), "%a %b %d %r", local);
    memcpy(message->Data.create_message.date, Time, strlen(Time));

    length = snprintf(NULL, 0, "INSERT INTO MSSGS (ROOM_ID, USER_ID, message, date) VALUES (%d, %d, '%s', '%s')", roomID, userID, text, Time);
    
    if (!(createmessageQuery = (char *)calloc(length, sizeof(char)))) {
        perror("Allocation fail!\n");
        message->status = ERROR;
        return;
    }

    sprintf(createmessageQuery,
        "INSERT INTO MSSGS (ROOM_ID, USER_ID, message, date) VALUES (%d, %d, '%s', '%s')", 
        roomID, userID, text, Time);

    rc = sqlite3_exec(db, createmessageQuery, 0, 0, &errMsg);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", errMsg);
        sqlite3_free(errMsg);
        message->status = ERROR;
    } else {
        sqlite3_prepare_v2(db, selectID, -1, &stmt, NULL);
        sqlite3_step(stmt);

        message->Data.create_message.message_id = sqlite3_column_int(stmt, 0);
    }

    free(createmessageQuery);
}

void CreateRoom(t_message *message, char *user, char *customer) 
{
    int length, userID, customerID, rc, ID;
    char *roomQuery;
    char *roomIDQuery = "SELECT MAX(ID) FROM ROOMS";
    char *errMsg = 0;
    sqlite3_stmt *stmt;

    message->API = CREATE_ROOM;
    message->status = SUCCESS;
    memcpy(message->Data.create_room.customer, customer, strlen(customer));

    userID = getUserID(user);
    customerID = getUserID(customer);

    length = snprintf(NULL, 0, "INSERT INTO ROOMS (USER_ID, CUSTOMER_ID, NAME) VALUES (%d, %d, '%s')", userID, customerID, customer);

    if (!(roomQuery = (char *)calloc(length, sizeof(char)))) {
        perror("Allocation fail!");
        message->status = ERROR;
        return;
    }

    sprintf(roomQuery, "INSERT INTO ROOMS (USER_ID, CUSTOMER_ID, NAME) VALUES (%d, %d, '%s')", userID, customerID, customer);
    rc = sqlite3_exec(db, roomQuery, 0, 0, &errMsg);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", errMsg);
        sqlite3_free(errMsg);
        message->status = ERROR;
    } else {
        sqlite3_prepare_v2(db, roomIDQuery, -1, &stmt, NULL);
        sqlite3_step(stmt);
        message->Data.create_room.id = sqlite3_column_int(stmt, 0);
    }

    free(roomQuery);
}

void UserSearch(t_message *message, char *searchText) 
{
    message->API = SEARCH_USER;
    message->status = SUCCESS;

    int lengthSearch, count = 0, size = 1;
    char *searchQuery;
    const unsigned char *result;
    sqlite3_stmt *stmt;

    message->Data.search_user.users = calloc(size, sizeof(char *));

    lengthSearch = snprintf(NULL, 0, "SELECT LOGIN FROM USRS WHERE LOGIN LIKE '%c%s%c';", '%', searchText, '%');

    if (!(searchQuery = (char *)calloc(lengthSearch, sizeof(char)))) {
        perror("Allocation failed!");
        message->status = ERROR;
        return;
    }

    sprintf(searchQuery, "SELECT LOGIN FROM USRS WHERE LOGIN LIKE '%c%s%c';", '%', searchText, '%');
    sqlite3_prepare_v2(db, searchQuery, -1, &stmt, NULL);

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

    free(searchQuery);
}

void SignUp(t_message *message, char *user_login, char *user_pass)
{
    message->API = SIGNUP;
    message->status = SUCCESS;
    sqlite3_stmt *stmt;
    char *signUpQuery, *zErrMsg = NULL;
    int lengthSignUp, ID, rc;

    if (checkLogin(user_login)) {
        message->status = SIGNUP_USER_EXIST;
        return;
    }

    char *idQuery = "SELECT MAX(ID) FROM USRS";
    sqlite3_prepare_v2(db, idQuery, -1, &stmt, NULL);
    sqlite3_step(stmt);

    ID = sqlite3_column_int(stmt, 0) + 1;

    lengthSignUp = snprintf(NULL, 0, "INSERT INTO USRS (ID, LOGIN, PASS) VALUES (%d, '%s', '%s');", ID, user_login, user_pass);

    if(!(signUpQuery = (char *)calloc(lengthSignUp, sizeof(char)))) {
        perror("allocation fail");
        message->status = ERROR;
        return;
    }

    sprintf(signUpQuery, "INSERT INTO USRS (ID, LOGIN, PASS) VALUES (%d, '%s', '%s');", ID, user_login, user_pass);

    rc = sqlite3_exec(db, signUpQuery, callback, 0, &zErrMsg);

    if( rc != SQLITE_OK ) {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
        message->status = ERROR;
    }

    free(signUpQuery);
}

void LogIn(t_message *message, char *user_login, char *user_pass)
{
    message->API = LOGIN;
    message->status = SUCCESS;
    sqlite3_stmt *stmt;
    const unsigned char *pass;
    char *query;

    if (!checkLogin(user_login)) {
        message->status = LOGIN_WRONG_USER;
        return;
    }
    
    int length = snprintf(NULL, 0, "SELECT PASS FROM USRS WHERE LOGIN  = '%s';", user_login);

    if(!(query = (char *) calloc(length, sizeof(char)))) {
        perror("allocation fail");
        message->status = ERROR;
        return;
    }

    sprintf(query, "SELECT PASS FROM USRS WHERE LOGIN  = '%s';", user_login);

    sqlite3_prepare_v2(db, query, -1, &stmt, NULL);
    sqlite3_step(stmt);

    pass = sqlite3_column_text(stmt, 0);

    if(strcmp((const char*)pass, user_pass))
        message->status = LOGIN_WRONG_PASS;

    free(query);
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
    char *idQuery;
    sqlite3_stmt *stmt;

    length = snprintf(NULL, 0, "SELECT ID FROM USRS WHERE LOGIN = '%s'", username);

    if (!(idQuery = (char *)calloc(length + 1, sizeof(char))))
        perror("Allocation error!\n");

    sprintf(idQuery, "SELECT ID FROM USRS WHERE LOGIN = '%s'", username);

    sqlite3_prepare_v2(db, idQuery, -1, &stmt, NULL);
    sqlite3_step(stmt);

    id = sqlite3_column_int(stmt, 0);

    sqlite3_finalize(stmt);

    free(idQuery);

    return id;
}

static int checkLogin(char *user_login)
{
    const unsigned char *checkLogin;
    char *checkQuery;
    sqlite3_stmt *stmt;

    int length = snprintf(NULL, 0, "SELECT LOGIN FROM USRS WHERE LOGIN = '%s';", user_login);

    if(!(checkQuery = (char *)calloc(length, sizeof(char)))) {
        perror("allocation fail");
        return 0;    
    }

    sprintf(checkQuery, "SELECT LOGIN FROM USRS WHERE LOGIN = '%s';", user_login);

    sqlite3_prepare_v2(db, checkQuery, -1, &stmt, NULL);
    sqlite3_step(stmt);

    checkLogin = sqlite3_column_text(stmt, 0);

    free(checkQuery);

    return checkLogin != NULL;
}

static int countSelectedRows(int userID) {
    int length, count;
    char *countQuery;
    sqlite3_stmt *stmt;

    length = snprintf(NULL, 0, "SELECT COUNT(*) FROM ROOMS WHERE USER_ID = %d", userID);
    if (!(countQuery = (char *)calloc(length, sizeof(char)))) {
        perror("Allocation fail!\n");
        return 0;
    }

    sprintf(countQuery, "SELECT COUNT(*) FROM ROOMS WHERE USER_ID = %d", userID);

    sqlite3_prepare_v2(db, countQuery, -1, &stmt, NULL);
    sqlite3_step(stmt);

    count = sqlite3_column_int(stmt, 0);

    free(countQuery);

    return count;
}
