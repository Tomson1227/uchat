#include "uchat_server.h"
#include "sqlite3.h"

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
    sqlite3_stmt *stmt;
    char *zErrMsg = 0;
    int rc;
    char *sql;
    //const char* data = "Callback function called";

    rc = sqlite3_open("test.db", &db);

    if( rc ) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        return(0);
    } else {
        fprintf(stderr, "Opened database successfully\n");
    }

    sql = "CREATE TABLE IF NOT EXISTS USRS(" \
    "ID    INT      PRIMARY KEY ," \
    "LOGIN CHAR(20)             ," \
    "PASS  CHAR(40)             );";
//
//    sql = "DROP TABLE USRS;";
//
    rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);

    if( rc != SQLITE_OK ){
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    } else {
        fprintf(stdout, "Table created successfully\n");
    }

//    sql = "INSERT INTO USRS (ID, LOGIN, PASS)"     \
//          "VALUES (1, 'mvrublevsk', 'qwerty228');" \
//          "INSERT INTO USRS (ID, LOGIN, PASS)"     \
//          "VALUES (2, 'opovshenko', 'qwerty322');";
//
//    rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
//
//    if( rc != SQLITE_OK ){
//        fprintf(stderr, "SQL error: %s\n", zErrMsg);
//        sqlite3_free(zErrMsg);
//    } else {
//        fprintf(stdout, "Records created successfully\n");
//    }

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

//    sql = "SELECT * from USRS";
//
//    rc = sqlite3_exec(db, sql, callback, (void*)data, &zErrMsg);
//
//    if( rc != SQLITE_OK ) {
//        fprintf(stderr, "SQL error: %s\n", zErrMsg);
//        sqlite3_free(zErrMsg);
//    } else {
//        fprintf(stdout, "Operation done successfully\n");
//    }
    const unsigned char *pass = (unsigned char*)malloc(40 * sizeof (unsigned char));
    char *Upass = (char*)malloc(40 * sizeof (char));

    sqlite3_prepare_v2(db, "SELECT PASS FROM USRS WHERE ID = 0", -1, &stmt, NULL);

    sqlite3_step(stmt);

    pass = sqlite3_column_text(stmt, 0);

    printf("Password:");
    scanf("%s", Upass);

    //diff types ¯\_(ツ)_/¯
    if (Upass == pass) {
        printf("Login successful\n");
    } else {
        printf("Wrong pasword\n");
    }

    //sqlite3_finalize(stmt);




    sqlite3_close(db);
    return 0;
}
