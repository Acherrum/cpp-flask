#include "cppflask/HttpServer.h"

#include <iostream>

#include "httplib/httplib.h"

#include "cppflask/IRoute.h"
#include "cppflask/JsonObject.h"

namespace {

std::string escape(const std::string& input) {
    auto result = std::string{};
    std::for_each(input.begin(), input.end(), [&](const auto& c) {
        if (c == '\\' || c == '"') {
            result += '\\';
        }
        result += c;
    });
    
    return result;
}

void removeTrailingComma(std::string& jsonString) {
    if (jsonString.back() == ',') {
        jsonString.back() = ' ';
    }
}

void parseCookies(const std::string& cookies, std::unordered_map<std::string, std::string>& jar) {

    std::stringstream stream(cookies);
    std::string keyValue;

    while (std::getline(stream, keyValue, ';')) {
        size_t start = keyValue.find_first_not_of(' ');
        if (start == std::string::npos) continue; 
        keyValue = keyValue.substr(start);

        // 3. Split the single pair by '='
        size_t assignmentPos = keyValue.find('=');
        if (assignmentPos != std::string::npos) {
            jar.emplace(keyValue.substr(0, assignmentPos), keyValue.substr(assignmentPos + 1));
        }
    }
}

std::string createHeaders(const httplib::Headers& headers) {
    auto jar = std::unordered_map<std::string, std::string>{};
    auto result = std::string{"\"headers\": {"};
    for (const auto& entry : headers) {
        if (entry.first == "Cookie") {
            parseCookies(entry.second, jar);
        }
        result += "\"" + entry.first + "\": \"" + escape(entry.second) + "\",";
    }
    removeTrailingComma(result);
    result += "}, \"cookies\": {";
    for (const auto& cookie : jar) {
        result += "\"" + cookie.first + "\": \"" + escape(cookie.second) + "\",";
    }
    removeTrailingComma(result);
    return result + "}";
}

std::string createGet(const httplib::Params& params) {
    auto result = std::string{"\"get\": {"};
    for (const auto& entry : params) {
        result += "\"" + entry.first + "\": \"" + escape(entry.second) + "\",";
    }
    removeTrailingComma(result);
    return result + "}";
}

std::string createForm(const httplib::MultipartFormData& form) {

    auto result = std::string{"\"form\": {"};
    for (const auto& field : form.fields) {
        result += "\"" + field.second.name + "\": \"" + escape(field.second.content) + "\",";
    }
    result += "\"files\": [";
    for (const auto& file : form.files) {
        result += "{\"name\": \"" + escape(file.second.name) + "\",";
        result += "\"content\": \"" + escape(file.second.content) + "\",";
        result += "\"filename\": \"" + escape(file.second.filename) + "\",";
        result += "\"content_type\": \"" + escape(file.second.content_type) + "\"},";
    }
    removeTrailingComma(result);
    return result + "]}";
}


cppflask::JsonObject generateDataFromRequest(const httplib::Request& req) {
    auto jsonString = std::string{"{"};
    jsonString += "\"path\": \"" + req.path + "\",";
    jsonString += createHeaders(req.headers) + ",";
    jsonString += createGet(req.params) + ",";
    jsonString += createForm(req.form);
    return cppflask::JsonObject{jsonString + "}"};

}

void setUpRoutes(httplib::Server& server, const cppflask::IRouter& router, const std::string& prefix = "/") {

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
                server.Get(prefix + route->getName(), handleRoute);
                break;
            }
            case cppflask::RouteType::POST: {
                server.Post(prefix + route->getName(), handleRoute);
                break;
            }
        }

        if (!route->getRoutes().empty()) {
            auto additionalPrefix = route->getName().empty() ? "" : route->getName() + '/';
            setUpRoutes(server, *route, prefix + additionalPrefix);
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