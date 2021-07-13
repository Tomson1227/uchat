#include "uchat_server.h"
#include "sqlite3.h"

static int callback(void *NotUsed, int argc, char **argv, char **azColName){
    int i;

    for(i = 0; i < argc; i++) {
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }

    printf("\n");
    return 0;
}

int Init_DB(char *name, const char *structure){
    sqlite3 *db;
    char *zErrMsg = 0;
    
    if(sqlite3_open(name, &db)) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 1;
    }

    const char user_table[] = "CREATE TABLE IF NOT EXIST users("
        "contact_id integer PRIMARY KEY,"
	    "first_name text NOT NULL,"
	    "last_name text NOT NULL,"
	    "email text NOT NULL UNIQUE,"
	    "phone text NOT NULL UNIQUE)";

    sqlite3_exec(db, user_table, callback, 0, &zErrMsg);


    if(sqlite3_exec(db, structure, callback, 0, &zErrMsg) != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }
  
    sqlite3_close(db);

    return 0;
}
