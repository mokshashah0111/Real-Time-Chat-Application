#include <iostream>
#include <boost/asio.hpp>

using namespace boost::asio;
using ip::tcp;

void sendMessage(const std::string& msg) {
    boost::asio::io_context io_context;
    tcp::socket socket(io_context);
    socket.connect(tcp::endpoint(ip::address::from_string("127.0.0.1"), 8080));
    write(socket, buffer(msg));
}

int main() {
    std::string username, password;
    std::cout << "Enter username: ";
    std::cin >> username;
    std::cout << "Enter password: ";
    std::cin >> password;

    sendMessage("REGISTER|" + username + "|" + password);

    std::string message;
    while (true) {
        std::cout << "Enter message: ";
        std::getline(std::cin, message);
        sendMessage("MESSAGE|" + username + "|receiver|" + message);
    }

    return 0;
}
