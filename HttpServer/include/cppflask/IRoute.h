#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <memory>

#include "cppflask/JsonObject.h"

namespace cppflask {
    enum class RouteType {
        GET, POST
    };

    class IRoute {
    public:
        virtual ~IRoute() = default;

        virtual RouteType getType() const = 0;
        virtual const std::string& getName() const = 0;
        virtual const std::string& getContentType() const = 0;
        virtual const std::unordered_map<std::string, std::string>& getHeaders() const = 0;
        virtual const std::vector<std::unique_ptr<IRoute>>& getRoutes() const = 0;

        virtual std::string generatePage(cppflask::JsonObject& requestData) = 0;
    };
}
