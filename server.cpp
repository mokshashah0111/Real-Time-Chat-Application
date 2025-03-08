#include <iostream>
#include <boost/asio.hpp>
#include <thread>
#include <sqlite3.h>
#include <vector>
#include <mutex>

using namespace boost::asio;
using ip::tcp;
std::mutex db_mutex;

sqlite3* getDBConnection() {
    sqlite3* db;
    sqlite3_open("chat.db", &db);
    return db;
}

bool registerUser(const std::string& username, const std::string& password) {
    std::lock_guard<std::mutex> lock(db_mutex);
    sqlite3* db = getDBConnection();
    sqlite3_stmt* stmt;
    
    const char* query = "INSERT INTO users (username, password) VALUES (?, ?);";
    sqlite3_prepare_v2(db, query, -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, password.c_str(), -1, SQLITE_STATIC);

    bool success = (sqlite3_step(stmt) == SQLITE_DONE);
    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return success;
}

bool authenticateUser(const std::string& username, const std::string& password) {
    std::lock_guard<std::mutex> lock(db_mutex);
    sqlite3* db = getDBConnection();
    sqlite3_stmt* stmt;
    
    const char* query = "SELECT password FROM users WHERE username = ?;";
    sqlite3_prepare_v2(db, query, -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);

    bool valid = false;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        std::string stored_password = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        valid = (stored_password == password);
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return valid;
}

void saveMessage(const std::string& sender, const std::string& receiver, const std::string& message) {
    std::lock_guard<std::mutex> lock(db_mutex);
    sqlite3* db = getDBConnection();
    sqlite3_stmt* stmt;

    const char* query = "INSERT INTO messages (sender, receiver, message) VALUES (?, ?, ?);";
    sqlite3_prepare_v2(db, query, -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, sender.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, receiver.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, message.c_str(), -1, SQLITE_STATIC);

    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    sqlite3_close(db);
}

void handleClient(tcp::socket socket) {
    try {
        boost::system::error_code error;
        char data[1024];
        
        while (true) {
            memset(data, 0, sizeof(data));
            size_t length = socket.read_some(buffer(data), error);

            if (error == boost::asio::error::eof) {
                std::cout << "Client disconnected." << std::endl;
                break;
            }

            std::string msg(data, length);
            std::cout << "Received: " << msg << std::endl;

            std::vector<std::string> parts;
            std::stringstream ss(msg);
            std::string token;
            while (getline(ss, token, '|')) {
                parts.push_back(token);
            }

            if (parts.size() < 2) continue;

            if (parts[0] == "REGISTER") {
                bool success = registerUser(parts[1], parts[2]);
                std::string response = success ? "REGISTER_SUCCESS" : "REGISTER_FAIL";
                write(socket, buffer(response));
            } 
            else if (parts[0] == "LOGIN") {
                bool valid = authenticateUser(parts[1], parts[2]);
                std::string response = valid ? "LOGIN_SUCCESS" : "LOGIN_FAIL";
                write(socket, buffer(response));
            } 
            else if (parts[0] == "MESSAGE") {
                saveMessage(parts[1], parts[2], parts[3]);
                std::string response = "MESSAGE_STORED";
                write(socket, buffer(response));
            }
        }
    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }
}

int main() {
    boost::asio::io_context io_context;
    tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), 8080));

    std::cout << "Server running on port 8080..." << std::endl;

    while (true) {
        tcp::socket socket(io_context);
        acceptor.accept(socket);
        std::thread(handleClient, std::move(socket)).detach();
    }

    return 0;
}
