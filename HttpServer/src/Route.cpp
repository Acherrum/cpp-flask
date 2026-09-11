#include "cppflask/Route.h"

namespace cppflask {
    Route::Route(const std::string &name, RouteType type, const std::string &contentType) :
        _type{type},
        _name{name},
        _contentType{contentType},
        _headers{{"Connection", "close"}},
        _routes{}
    {
    }

    Route::~Route() = default;

    RouteType Route::getType() const {

        return _type;
    }

    const std::string &Route::getName() const {

        return _name;
    }

    const std::string &Route::getContentType() const {

        return _contentType;
    }

    const std::unordered_map<std::string, std::string> &Route::getHeaders() const {

        return _headers;
    }

    const std::vector<std::unique_ptr<IRoute>> &Route::getRoutes() const {

        return _routes;
    }

    void Route::setHeader(const std::string &key, const std::string &value) {

        std::ignore = _headers.emplace(key, value);
    }

    void Route::addRoute(std::unique_ptr<IRoute> newRoute) {

        _routes.emplace_back(std::move(newRoute));
    }
}