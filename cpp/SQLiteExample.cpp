#include <iostream>
#include <string>
#include <boost/scope_exit.hpp>

#include "sqlite3.h"


int main() {
    sqlite3 *db = nullptr;
    sqlite3_stmt *stmt = nullptr;
    BOOST_SCOPE_EXIT_ALL(&) {
        // it's OK to call sqlite3_finalize & sqlite3_close with a null pointer
        sqlite3_finalize(stmt);
        sqlite3_close(db);
    };

    // create/open a new database
    sqlite3_open("test.db", &db);

    //
    // create table
    //
    std::string query =
        "CREATE TABLE IF NOT EXISTS"
        " USER(ID INTEGER PRIMARY KEY, NAME TEXT NOT NULL, AGE INTEGER);";

    // sqlite3_exec is a convenient way to execute query
    sqlite3_exec(db, query.c_str(), nullptr, nullptr, nullptr);


    //
    // insert new records to database
    //
    query = "INSERT INTO USER (NAME, AGE) VALUES (?, ?);";
    sqlite3_prepare_v2(db, query.c_str(), query.size(), &stmt, nullptr);

    std::string userName = "Name 1";
    int userAge = 30;
    sqlite3_bind_text(stmt, 1, userName.c_str(), userName.size(), nullptr); // bind NAME
    sqlite3_bind_int64(stmt, 2, userAge);   // bind AGE
    
    // execute prepared query
    sqlite3_step(stmt);

    // get id from last insert
    std::cout << "Last id : " << sqlite3_last_insert_rowid(db) << "\n\n";

    // must reset and clear bindings to reuse prepared query
    sqlite3_reset(stmt);
    sqlite3_clear_bindings(stmt);

    userName = "Name 2";
    userAge = 40;
    sqlite3_bind_text(stmt, 1, userName.c_str(), userName.size(), nullptr); // bind NAME
    sqlite3_bind_int64(stmt, 2, 40);    // bind AGE
    
    sqlite3_step(stmt);

    std::cout << "Last id : " << sqlite3_last_insert_rowid(db) << "\n\n";

    // clear prepared query after using
    sqlite3_finalize(stmt);
    stmt = nullptr;


    //
    // list all records
    //
    query = "SELECT ID, NAME, AGE FROM USER;";
    sqlite3_prepare_v2(db, query.c_str(), query.size(), &stmt, nullptr);

    // step through rows
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        auto id = sqlite3_column_int64(stmt, 0);    // read ID
        auto name = sqlite3_column_text(stmt, 1);   // read NAME
        auto age = sqlite3_column_int64(stmt, 2);   // read AGE

        std::cout
            << "ID   : "    << id   << "\n"
            << "NAME : "    << name << "\n"
            << "AGE  : "    << age  << "\n\n";
    }

    sqlite3_finalize(stmt);
    stmt = nullptr;


    //
    // query with criterion
    //
    query = "SELECT NAME, AGE FROM USER WHERE ID = ?;";
    sqlite3_prepare_v2(db, query.c_str(), query.size(), &stmt, nullptr);
    sqlite3_bind_int64(stmt, 1, 1); // bind ID
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        auto name = sqlite3_column_text(stmt, 0);   // read NAME
        auto age = sqlite3_column_int64(stmt, 1);   // read AGE

        std::cout
            << "NAME : " << name << "\n"
            << "AGE  : " << age << "\n\n";
    }

    sqlite3_finalize(stmt);
    stmt = nullptr;


    //
    // update
    //
    const std::string new_name = "New name";
    query = "UPDATE USER SET NAME = ?, AGE = ? WHERE ID = ?;";
    sqlite3_prepare_v2(db, query.c_str(), query.size(), &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, new_name.c_str(), new_name.size(), nullptr); // bind NAME
    sqlite3_bind_int64(stmt, 2, 10);    // bind AGE
    sqlite3_bind_int64(stmt, 3, 1);     // bind ID
    
    sqlite3_step(stmt);

    sqlite3_finalize(stmt);
    stmt = nullptr;
}
