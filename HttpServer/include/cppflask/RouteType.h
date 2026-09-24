#pragma once

#include <string>
#include <unordered_map>

namespace cppflask {
    enum class RouteType {
        GET, POST, PUT, DEL
    };

    inline std::string toString(const RouteType val) {
        static std::unordered_map<RouteType, std::string> serialized{
            { RouteType::GET, "GET" },
            { RouteType::POST, "POST" },
            { RouteType::PUT, "PUT" },
            { RouteType::DEL, "DELETE" }
        };
        auto result = serialized.find(val);
        if (result != serialized.end()) {
            return result->second;
        }
        return "";
    }
}