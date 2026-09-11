#include "cppflask/HttpServer.h"

#include <iostream>

#include "httplib/httplib.h"

#include "cppflask/IRouter.h"
#include "cppflask/JsonObject.h"

namespace {

    cppflask::JsonObject generateDataFromRequest(const httplib::Request& req) {
        auto jsonString = std::string{"{"};
        jsonString += "\"path\": \"" + req.path + "\",";
        jsonString += "\"body\": \"" + req.body + "\",";
        jsonString += "\"form\": {";
            for (const auto& field : req.form.fields) {
                jsonString += "\"" + field.second.name + "\": \"" + field.second.content + "\",";
            }
        if (jsonString.back() == ',') {
            jsonString.back() = ' ';
        }
        jsonString += "}";
        return cppflask::JsonObject{jsonString + "}"};

    }

    void setUpRoutes(httplib::Server& server, const cppflask::IRouter& router) {

    for (const auto& route : router.getRoutes()) {
        auto handleRoute = [&](const httplib::Request& req, httplib::Response& res) {

            auto data = generateDataFromRequest(req);
            res.set_content(route->generatePage(data), route->getContentType());
            for (const auto& header : route->getHeaders()) {
                res.set_header(header.first, header.second);
            }
        };

        switch (route->getType()) {
            case cppflask::RouteType::GET: {
                server.Get("/" + route->getName(), handleRoute);
                break;
            }
            case cppflask::RouteType::POST: {
                server.Post("/" + route->getName(), handleRoute);
                break;
            }
        }
    }
    }
}

namespace cppflask {
    HttpServer::HttpServer(const IRouter& router) :
        _server{std::make_unique<httplib::Server>()} {

        setUpRoutes(*_server, router);
    }

    HttpServer::~HttpServer() {

        if (_isStarted) {
            _server->stop();
        }
    }

    void HttpServer::start(int port) {

        _port = port;
        std::cout << "Starting @ localhost:" << std::to_string(port) << std::endl;
        _serverThread = std::thread(&HttpServer::listen, this);
    }

    void HttpServer::stop() {

        std::cout << "Stopping server" << std::endl;
        _server->stop();
        _serverThread.join();
        _isStarted = false;
        std::cout << "Stopped" << std::endl;
    }

    bool HttpServer::isRunning() const {

        return _isStarted;
    }

    void HttpServer::listen() {

        _isStarted = true;
        std::cout << "Listening" << std::endl;
        _server->listen("0.0.0.0", _port);
    }
}