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
            // fprintf(stderr, "SQL error: %s\n", zErrMsg);
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
//    const char* data = "Callback function called";

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

    sql = "SELECT * FROM USRS";

    rc = sqlite3_exec(server->db, sql, callback, 0, &zErrMsg);

    if( rc != SQLITE_OK ){
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    } else {
        fprintf(stdout, "Table created successfully\n");
    }

//    sign_up(server->db, "OOLLL", "qwerty228");
//    login(server->db, "OOLLL", "qwerty228");


    // sqlite3_close(db);
}
