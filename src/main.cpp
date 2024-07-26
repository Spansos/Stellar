#include <iostream>
#include <string>

#include "common.hpp"
#include "client.hpp"
#include "server.hpp"


int main() {
    std::string input;
    std::cout << "client or server?\n";
    std::cin >> input;

    if (input == "client") {
        std::string address_str;
        int port;
        std::cout << "ip address: ";
        std::cin >> address_str;
        std::cout << "port: ";
        std::cin >> port;
        Client client{address_str, port};
        client.run();
    } else if (input == "server") {
        Server server{30, sf::Socket::AnyPort};
        server.run();
    } else {
        throw;
    }
}