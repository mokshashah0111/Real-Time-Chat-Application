#include <sqlite3.h>
#include <iostream>

void createDatabase() {
    sqlite3* db;
    char* errMsg = nullptr;

    if (sqlite3_open("chat.db", &db) != SQLITE_OK) {
        std::cerr << "Error opening database: " << sqlite3_errmsg(db) << std::endl;
        return;
    }

    const char* usersTable = "CREATE TABLE IF NOT EXISTS users ("
                             "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                             "username TEXT UNIQUE NOT NULL,"
                             "password TEXT NOT NULL);";

    const char* messagesTable = "CREATE TABLE IF NOT EXISTS messages ("
                                "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                                "sender TEXT NOT NULL,"
                                "receiver TEXT NOT NULL,"
                                "message TEXT NOT NULL,"
                                "timestamp DATETIME DEFAULT CURRENT_TIMESTAMP);";

    if (sqlite3_exec(db, usersTable, nullptr, nullptr, &errMsg) != SQLITE_OK ||
        sqlite3_exec(db, messagesTable, nullptr, nullptr, &errMsg) != SQLITE_OK) {
        std::cerr << "Error creating tables: " << errMsg << std::endl;
        sqlite3_free(errMsg);
    } else {
        std::cout << "Database initialized successfully!" << std::endl;
    }

    sqlite3_close(db);
}

int main() {
    createDatabase();
    return 0;
}
