#include <iostream>

#include "cppflask/HttpServer.h"
#include "Router.h"

int main() {
    std::cout << "Creating the router." << std::endl;
    Router router{};
    std::cout << "Plugging router into server." << std::endl;
    cppflask::HttpServer::run(router);

    return 0;
}