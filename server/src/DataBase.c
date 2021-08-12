#include "uchat_server.h"

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




int getUserID(sqlite3 *db, char *username) {
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

int createMessage(sqlite3 *db, int roomID) {
    int length, rc, ID;
    char *createMessageQuery, *errMsg;
    char *selectID = "SELECT MAX(ID) FROM MSSGS";
    sqlite3_stmt *stmt;

    length = snprintf(NULL, 0, "INSERT INTO MSSGS (ROOM_ID) VALUES (%d)", roomID);
    if (!(createMessageQuery = (char *)calloc(length, sizeof(char))))
        perror("Allocation fail!\n");

    sprintf(createMessageQuery, "INSERT INTO MSSGS (ROOM_ID) VALUES (%d)", roomID);

    rc = sqlite3_exec(db, createMessageQuery, 0, 0, &errMsg);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", errMsg);

        sqlite3_free(errMsg);
        free(createMessageQuery);

        return 0;
    } else {
        sqlite3_prepare_v2(db, selectID, -1, &stmt, NULL);
        sqlite3_step(stmt);

        ID = sqlite3_column_int(stmt, 0);

        free(createMessageQuery);

        return ID;
    }

}


int createRoom(sqlite3 *db, char *user, char *customer) {
    int length, userID, customerID, rc, ID;
    char *roomQuery;
    char *roomIDQuery = "SELECT MAX(ID) FROM ROOMS";
    char *errMsg = 0;
    sqlite3_stmt *stmt;

    userID = getUserID(db, user);

    customerID = getUserID(db, customer);


    length = snprintf(NULL, 0, "INSERT INTO ROOMS (USER_ID, CUSTOMER_ID) VALUES (%d, %d)", userID, customerID);
    if (!(roomQuery = (char *)calloc(length, sizeof(char))))
        perror("Allocation fail!");

    sprintf(roomQuery, "INSERT INTO ROOMS (USER_ID, CUSTOMER_ID) VALUES (%d, %d)", userID, customerID);

    rc = sqlite3_exec(db, roomQuery, 0, 0, &errMsg);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", errMsg);

        sqlite3_free(errMsg);
        free(roomQuery);

        return 0;
    } else {
        sqlite3_prepare_v2(db, roomIDQuery, -1, &stmt, NULL);
        sqlite3_step(stmt);

        ID = sqlite3_column_int(stmt, 0);

        free(roomQuery);

        return ID;
    }
}




int checkLogin(sqlite3 *db, char *user_login) {
    const unsigned char *checkLogin;
    char *checkQuery;
    int length;
    sqlite3_stmt *stmt;

    length = snprintf(NULL, 0, "SELECT LOGIN FROM USRS WHERE LOGIN = '%s';", user_login);
    if(!(checkQuery = (char *)calloc(length, sizeof(char))))
        perror("allocation fail");

    sprintf(checkQuery, "SELECT LOGIN FROM USRS WHERE LOGIN = '%s';", user_login);

    sqlite3_prepare_v2(db, checkQuery, -1, &stmt, NULL);
    sqlite3_step(stmt);

    checkLogin = sqlite3_column_text(stmt, 0);

    if (checkLogin == NULL) {
        free(checkQuery);
        return 0;
    } else {
        free(checkQuery);
        return 1;
    }
}




char **userSearch(sqlite3 *db, char *searchText) {
    int lengthSearch, i = 0, size = 1;
    char *searchQuery;
    const unsigned char *result;
    char **resultArr = (char **)malloc(size * sizeof(char *));

    sqlite3_stmt *stmt;

    lengthSearch = snprintf(NULL, 0, "SELECT LOGIN FROM USRS WHERE LOGIN LIKE '%c%s%c';", '%', searchText, '%');

    if (!(searchQuery = (char *)calloc(lengthSearch, sizeof(char))))
        perror("Allocation failed!");

    sprintf(searchQuery, "SELECT LOGIN FROM USRS WHERE LOGIN LIKE '%c%s%c';", '%', searchText, '%');


    sqlite3_prepare_v2(db, searchQuery, -1, &stmt, NULL);

    while (1) {
        sqlite3_step(stmt);
        result = sqlite3_column_text(stmt, 0);

        if (result != NULL) {
            resultArr[i] = (char *)result;
            resultArr = realloc(resultArr, ++size * sizeof(char *));
        } else
            break;

        ++i;
    }
    resultArr[i] = NULL;

    free(searchQuery);

    return resultArr;
}

t_rs_status sign_up(sqlite3 *db, char *user_login, char *user_pass)
{
    t_rs_status status = SIGNUP_OK;
    sqlite3_stmt *stmt;
    char *zErrMsg = NULL;
    int lengthSignUp, ID, rc;

    if (checkLogin(db, user_login) == 0) {

        char *idQuery = "SELECT MAX(ID) FROM USRS";
        sqlite3_prepare_v2(db, idQuery, -1, &stmt, NULL);
        sqlite3_step(stmt);

        ID = sqlite3_column_int(stmt, 0) + 1;


        lengthSignUp = snprintf(NULL, 0, "INSERT INTO USRS (ID, LOGIN, PASS) VALUES (%d, '%s', '%s');", ID, user_login, user_pass);
        char *signUpQuery;

        if(!(signUpQuery = (char *)calloc(lengthSignUp, sizeof(char))))
            perror("allocation fail");

        sprintf(signUpQuery, "INSERT INTO USRS (ID, LOGIN, PASS) VALUES (%d, '%s', '%s');", ID, user_login, user_pass);

        rc = sqlite3_exec(db, signUpQuery, callback, 0, &zErrMsg);

        if( rc != SQLITE_OK ) {
            status = SINGUP_FAIL;
            fprintf(stderr, "SQL error: %s\n", zErrMsg);
            sqlite3_free(zErrMsg);
        }

        free(signUpQuery);
    } else if (checkLogin(db, user_login) == 1)
        status = SIGNUP_USER_EXIST;

    return status;
}

t_rs_status login(sqlite3 *db, char *user_login, char *user_pass) {
    t_rs_status status = LOGIN_OK;
    sqlite3_stmt *stmt;
    const unsigned char *pass;
    char *query;

    if (checkLogin(db, user_login) == 0) {
        status = LOGIN_WRONG_USER;
        return status;
    }

    int length = snprintf(NULL, 0, "SELECT PASS FROM USRS WHERE LOGIN  = '%s';", user_login);

    if(!(query = (char *) calloc(length, sizeof(char))))
        perror("allocation fail");

    sprintf(query, "SELECT PASS FROM USRS WHERE LOGIN  = '%s';", user_login);

    sqlite3_prepare_v2(db, query, -1, &stmt, NULL);
    sqlite3_step(stmt);

    pass = sqlite3_column_text(stmt, 0);

    if (strcmp((const char*)pass, user_pass))
        status = LOGIN_WRONG_PASS;

    free(query);
    return status;
}

void Init_DB(t_server * server)
{
    char *zErrMsg = 0;
    int rc;
    char *sql;

    rc = sqlite3_open("uchat.db", &server->db);

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

//    sql = "DROP TABLE USRS;";

//    sql = "SELECT * FROM USRS";

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
    "MESSAGE TEXT);";

//    sql = "SELECT * FROM MSSGS";

    rc = sqlite3_exec(server->db, sql, callback, 0, &zErrMsg);

    if( rc != SQLITE_OK ){
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    } else {
        fprintf(stdout, "Messages table created successfully\n");
    }

}

