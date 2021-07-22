#include "uchat_server.h"
#include "sqlite3.h"

char* itoa(int val, int base){

    static char buf[32] = {0};

    int i = 30;

    for(; val && i ; --i, val /= base)

        buf[i] = "0123456789abcdef"[val % base];

    return &buf[i+1];

}

static int callback(void *data, int argc, char **argv, char **azColName) {
    int i;
    fprintf(stderr, "%s: ", (const char*)data);

    for(i = 0; i<argc; i++){
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }

    printf("\n");
    return 0;
}

int Init_DB(char *name, const char *structure){
//    sqlite3 *db;
//    char *zErrMsg = 0;
//
//    if(sqlite3_open(name, &db)) {
//        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
//        sqlite3_close(db);
//        return 1;
//    }
//
//    const char user_table[] = "CREATE TABLE IF NOT EXIST users("
//        "contact_id integer PRIMARY KEY,"
//	    "first_name text NOT NULL,"
//	    "last_name text NOT NULL,"
//	    "email text NOT NULL UNIQUE,"
//	    "phone text NOT NULL UNIQUE)";
//
//    sqlite3_exec(db, user_table, callback, 0, &zErrMsg);
//
//
//    if(sqlite3_exec(db, structure, callback, 0, &zErrMsg) != SQLITE_OK) {
//        fprintf(stderr, "SQL error: %s\n", zErrMsg);
//        sqlite3_free(zErrMsg);
//    }
//
//    sqlite3_close(db);
//
//    return 0;
    sqlite3 *db;

    char *zErrMsg = 0;
    int rc;
    char *sql;
    const char* data = "Callback function called";

    rc = sqlite3_open("test.db", &db);

    if( rc ) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        return(0);
    } else {
        fprintf(stderr, "Opened database successfully\n");
    }

    sql = "CREATE TABLE IF NOT EXISTS USRS(" \
    "ID INT PRIMARY KEY," \
    "LOGIN TEXT," \
    "PASS  TEXT);";

//    sql = "DROP TABLE USRS;";

    rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);

    if( rc != SQLITE_OK ){
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    } else {
        fprintf(stdout, "Table created successfully\n");
    }

    sql = "INSERT INTO USRS (ID, LOGIN, PASS)"     \
          "VALUES (1, 'mvrublevsk', 'qwerty228');" \
          "INSERT INTO USRS (ID, LOGIN, PASS)"     \
          "VALUES (2, 'opovshenko', 'qwerty322');";

    rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);

    if( rc != SQLITE_OK ){
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    } else {
        fprintf(stdout, "Records created successfully\n");
    }

//    sql = "DELETE from USRS;";
//
//    rc = sqlite3_exec(db, sql, callback, (void*)data, &zErrMsg);
//
//    if( rc != SQLITE_OK ) {
//        fprintf(stderr, "SQL error: %s\n", zErrMsg);
//        sqlite3_free(zErrMsg);
//    } else {
//        fprintf(stdout, "Operation done successfully\n");
//    }

//    sql = "INSERT INTO USRS (ID, LOGIN, PASS)" \
//          "VALUES (1, 'mvrublevsk', 'qwerty228');";
//
//    rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
//
//    if( rc != SQLITE_OK ){
//        fprintf(stderr, "SQL error: %s\n", zErrMsg);
//        sqlite3_free(zErrMsg);
//    } else {
//        fprintf(stdout, "Records created successfully\n");
//    }

//    sql = "SELECT PASS from USRS WHERE LOGIN='mvrublevsk'";
//
//    rc = sqlite3_exec(db, sql, callback, (void*)data, &zErrMsg);
//
//    if( rc != SQLITE_OK ) {
//        fprintf(stderr, "SQL error: %s\n", zErrMsg);
//        sqlite3_free(zErrMsg);
//    } else {
//        fprintf(stdout, "Operation done successfully\n");
//    }

    sqlite3_stmt *stmt;

/// SIGN UP
//    unsigned int lastID;
//    char query[100] = "INSERT INTO USRS (ID, LOGIN, PASS) VALUES (";
//    const unsigned char *checkLog;
//    char *coma = ",";
//    char *end = ");";
//    char *log = "'mark'";
//    char *pas = "'123qwe'";
//    char *ID;
//
//    /************Example*********/
//
////    int length = snprintf(NULL, 0, "INSERT INTO USRS (ID, LOGIN, PASS) VALUES (%s, %s, %s);", ID, log, pass);
////    char *new_request = (char *) calloc(length, sizeof(char));
////    sprintf(new_request, "INSERT INTO USRS (ID, LOGIN, PASS) VALUES (%s, %s, %s);", ID, log, pass);
////
////    free(new_request);
//    /************Example*********/
//
//    sql = "SELECT MAX(ID) FROM USRS";
//
//    sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
//    sqlite3_step(stmt);
//
//    lastID = sqlite3_column_int(stmt, 0);
//    lastID++;
//    ID = itoa(lastID, 10);
//
//    sqlite3_reset(stmt);
//
//    char loginCheckQuery[70] = "SELECT LOGIN FROM USRS WHERE LOGIN =";
//    strcat(loginCheckQuery, log);
//    printf("%s\n", loginCheckQuery);
//
//    sqlite3_prepare_v2(db, loginCheckQuery, -1, &stmt, NULL);
//    sqlite3_step(stmt);
//
//    checkLog = sqlite3_column_text(stmt, 0);
//    printf("%s\n", checkLog);
//
//    if (checkLog == (unsigned char*)log) {
//        fprintf(stderr, "Login already exists\n");
//        return 0;
//    }
//
//
//    strcat(query, ID);
//    strcat(query, coma);
//    strcat(query, log);
//    strcat(query, coma);
//    strcat(query, pas);
//    strcat(query, end);
//
//    //printf("%s\n", query);
////    rc = sqlite3_exec(db, query, callback, 0, &zErrMsg);
////
////    if( rc != SQLITE_OK ){
////        fprintf(stderr, "SQL error: %s\n", zErrMsg);
////        sqlite3_free(zErrMsg);
////    } else {
////        fprintf(stdout, "Records created successfully\n");
////    }
//
//    sqlite3_reset(stmt);
/// END OF SIGN UP


/// LOGIN :
    char login[] = "'mvrublevsk'";
    char *testPass = "qwerty228";

    const unsigned char *pass = (unsigned char*)malloc(40 * sizeof (unsigned char));
    const unsigned char *Upass = (unsigned char*)malloc(40 * sizeof (unsigned char));
    char firstPart[80] = "SELECT PASS FROM USRS WHERE LOGIN =";

    strcat(firstPart, login);

    sqlite3_prepare_v2(db, firstPart, -1, &stmt, NULL);

    sqlite3_step(stmt);

    pass = sqlite3_column_text(stmt, 0);

    Upass = (const unsigned char*)testPass;

    printf("Your Password: %s\n", Upass);
    printf("Password: %s\n", pass);



    if (memcmp(pass, Upass, 10) == 0) {
        printf("Login successful\n");
    } else {
        printf("Wrong pasword\n");
    }

    sqlite3_reset(stmt);

/// END OF LOGIN

    sqlite3_close(db);
    return 0;
}
