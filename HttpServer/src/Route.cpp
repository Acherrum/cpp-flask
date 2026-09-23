#include "cppflask/Route.h"

namespace cppflask {
    Route::Route(const std::string &name, RouteType type, const std::string &contentType) :
        _type{type},
        _name{name},
        _contentType{contentType},
        _headers{}
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

    const std::unordered_multimap<std::string, std::string> &Route::getHeaders() const {

        return _headers;
    }

    void Route::setHeader(const std::string &key, const std::string &value) {

        std::ignore = _headers.emplace(key, value);
    }

    void Route::setCookie(const std::string &key, const std::string &value, unsigned long lifetime) {

        setHeader("Set-Cookie", key +"=" + value + "; Max-Age="+ std::to_string(lifetime) +"; Path=/; SameSite=Strict; HttpOnly");
    }
}