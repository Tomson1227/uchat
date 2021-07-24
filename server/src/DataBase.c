#include "uchat_server.h"

typedef enum s_response_status
{
    LOGIN_OK,
    LOGIN_WRONG_USER,
    LOGIN_WRONG_PASS,
    SIGNUP_OK,
    SIGNUP_USER_EXIST,
    SINGUP_FAIL
}   t_response_status;

char* itoa(int val, int base){

    static char buf[32] = {0};

    int i = 30;

    for(; val && i ; --i, val /= base)

        buf[i] = "0123456789abcdef"[val % base];

    return &buf[i+1];

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

void sign_up(sqlite3 *db, char *user_login, char *user_pass) 
{
    t_response_status status;
    sqlite3_stmt *stmt;
    const unsigned char *checkLogin;
    char *zErrMsg = 0;
    int ID, rc;
    int length;
    int lengthSignUp;

    length = snprintf(NULL, 0, "SELECT LOGIN FROM USRS WHERE LOGIN = '%s';", user_login);
    char *checkQuery;

    if(!(checkQuery = (char *)calloc(length, sizeof(char))))
        perror("allocation fail");

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

        ID = sqlite3_column_int(stmt, 0) + 1;

        // printf("%d\n", ID);

        sqlite3_reset(stmt);

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
        } else
            status = SINGUP_OK;

        
        free(signUpQuery);
    } else if (strcmp((char *)checkLogin, user_login) == 0)
        status = SINGUP_USER_EXIST;

    char *massege = send_rs_sign_up_server(status);
    //SEMD MASSEGE
    free(checkQuery);
    free(checkLogin);
    free(massege);
}

void login(sqlite3 *db, char *user_login, char *user_pass) {
    t_response_status status;
    const unsigned char *pass = (unsigned char *)calloc(20, sizeof(unsigned char));
    int lengthL = snprintf(NULL, 0, "SELECT PASS FROM USRS WHERE LOGIN  = '%s';", user_login);
    char *query;
    
    if(!(query = (char *) calloc(lengthL, sizeof(char))))
        perror("allocation fail");

    sqlite3_stmt *stmt;

    sprintf(query, "SELECT PASS FROM USRS WHERE LOGIN  = '%s';", user_login);

    sqlite3_prepare_v2(db, query, -1, &stmt, NULL);
    sqlite3_step(stmt);

    pass = sqlite3_column_text(stmt, 0);

    if (strcmp((const char*)pass, user_pass) == 0) {
        printf("Login successful\n");
        status = LOGIN_OK;
    } else {
        printf("Wrong pasword\n");
        status = LOGIN_WRONG_PASSWORD;
    }

    free(query);
    free(pass);
}

void Init_DB(t_server * server)
{
    char *zErrMsg = 0;
    int rc;
    char *sql;
    const char* data = "Callback function called";

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

    rc = sqlite3_exec(server->db, sql, callback, 0, &zErrMsg);

    if( rc != SQLITE_OK ){
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    } else {
        fprintf(stdout, "Table created successfully\n");
    }

    // sign_up(db, "mark", "qwerty228");
    // login(db, "mark", "qwerty228");


    // sqlite3_close(db);
}
