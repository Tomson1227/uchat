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

int sign_up(sqlite3 *db, char *user_login, char *user_pass) {
    sqlite3_stmt *stmt;
    const unsigned char *checkLogin;
    char *zErrMsg = 0;
    int ID, rc;
    int length;
    int lengthSignUp;

    length = snprintf(NULL, 0, "SELECT LOGIN FROM USRS WHERE LOGIN = '%s';", user_login);
    char *checkQuery = (char *)calloc(length, sizeof(char));
    sprintf(checkQuery, "SELECT LOGIN FROM USRS WHERE LOGIN = '%s';", user_login);

    sqlite3_prepare_v2(db, checkQuery, -1, &stmt, NULL);
    sqlite3_step(stmt);

    checkLogin = sqlite3_column_text(stmt, 0);
    printf("%s %s\n", checkLogin, user_login);

    if (checkLogin == NULL) {
        sqlite3_reset(stmt);

        char *idQuery = "SELECT MAX(ID) FROM USRS";
        sqlite3_prepare_v2(db, idQuery, -1, &stmt, NULL);
        sqlite3_step(stmt);

        ID = sqlite3_column_int(stmt, 0);

        printf("%d\n", ID);

        if (ID == (int)NULL)
            ID = 1;
        else
            ID++;

        sqlite3_reset(stmt);

        lengthSignUp = snprintf(NULL, 0, "INSERT INTO USRS (ID, LOGIN, PASS) VALUES (%d, '%s', '%s');", ID, user_login, user_pass);
        char *signUpQuery = (char *)calloc(lengthSignUp, sizeof(char));
        sprintf(signUpQuery, "INSERT INTO USRS (ID, LOGIN, PASS) VALUES (%d, '%s', '%s');", ID, user_login, user_pass);

        rc = sqlite3_exec(db, signUpQuery, callback, 0, &zErrMsg);

        if( rc != SQLITE_OK ){
            fprintf(stderr, "SQL error: %s\n", zErrMsg);
            return 0;
            sqlite3_free(zErrMsg);
        } else {
            fprintf(stdout, "Records created successfully\n");
            return 1;
        }
    } else if (strcmp((char *)checkLogin, user_login) == 0) {
        fprintf(stderr, "%s\n", "User already exists");
        return 0;
    }
    return 0;
}

int login(sqlite3 *db, char *user_login, char *user_pass) {

    const unsigned char *pass = (unsigned char *)calloc(20, sizeof(unsigned char));
    int lengthL = snprintf(NULL, 0, "SELECT PASS FROM USRS WHERE LOGIN  = '%s';", user_login);
    char *query = (char *)calloc(lengthL, sizeof(char));

    sqlite3_stmt *stmt;

    sprintf(query, "SELECT PASS FROM USRS WHERE LOGIN  = '%s';", user_login);

    sqlite3_prepare_v2(db, query, -1, &stmt, NULL);
    sqlite3_step(stmt);

    pass = sqlite3_column_text(stmt, 0);

    if (strcmp((const char*)pass, user_pass) == 0) {
        printf("Login successful\n");
        return 1;
    } else {
        printf("Wrong pasword\n");
        return 0;
    }
}

int Init_DB(char *name, const char *structure){
    sqlite3 *db;

    char *zErrMsg = 0;
    int rc;
    char *sql;
    const char* data = "Callback function called";

    rc = sqlite3_open("uchat.db", &db);

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

    

    sign_up(db, "mark", "qwerty228");
    login(db, "mark", "qwerty228");


    sqlite3_close(db);
    return 0;
}
