#include <iostream>

#include "cppflask/HttpServer.h"
#include "cppflask/html/FilterRegistry.h"
#include "cppflask/JsonObject.h"
#include "Router.h"

int main() {
    cppflask::html::FilterRegistry::registerFilter("pingPong", [](cppflask::JsonObject& data) {
        auto result = std::string{};
        auto val = std::stoll(data.getValueAsString(""));
        if (val % 3 == 0) result += "Ping";
        if (val % 5 == 0) result += "Pong";
        if (result.empty()) result = std::to_string(val);

        return result;
    });

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