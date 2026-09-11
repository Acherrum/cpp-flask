#include <iostream>

#include "cppflask/HttpServer.h"
#include "Router.h"

int main() {
    std::cout << "Creating the router." << std::endl;
    Router router{};
    std::cout << "Plugging router into server." << std::endl;
    cppflask::HttpServer server{router};

    std::cout << "Starting the server." << std::endl;
    server.start();

    std::string keepRunning{};
    while(keepRunning.empty()) {
        std::getline(std::cin, keepRunning);
    }

    server.stop();
    return 0;
}