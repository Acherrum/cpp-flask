#include "cppflask/HttpServer.h"
#include "cppflask/Logger.h"

#include "Router.h"

int main() {
    cppflask::DEFAULT_LOG_LEVEL = cppflask::LogLevel::Debug;

    auto logger = cppflask::Logger{"HelloWorld"};
    logger.log(cppflask::LogLevel::Debug, "Creating the router.");
    Router router{};
    logger.log(cppflask::LogLevel::Debug, "Plugging router into server.");
    cppflask::HttpServer::run(router);

    return 0;
}