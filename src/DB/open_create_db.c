#include <stdio.h>
//#include "sqlite3.h"

#include <sqlite3.h>


const char *SQL = "CREATE TABLE IF NOT EXISTS users(userid INTEGER PRIMARY KEY, first name TEXT, last name TEXT, username TEXT, password TEXT, loggin TEXT);";

int main() {

    sqlite3 *db = 0;
    char *err = 0;


    if( sqlite3_open("my_test_db.dblite", &db) )
        fprintf(stderr, "Ошибка открытия/создания БД: %s\n", sqlite3_errmsg(db));
    else if(sqlite3_exec(db, SQL, 0, 0, &err)) {
        fprintf(stderr, "Ошибка SQL: %sn", err);
        sqlite3_free(err);
    }
    sqlite3_close(db);
    return 0;
}


