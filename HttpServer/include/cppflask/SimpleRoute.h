#pragma once

#include "cppflask/Route.h"

#include <functional>
#include <string>

namespace cppflask {
    class SimpleRoute : public Route {
    public:
        explicit SimpleRoute(const std::string& name,
            std::function<std::string(JsonObject&)> routeResult,
            RouteType type = RouteType::GET,
            const std::string& contentType = "text/html");
        ~SimpleRoute() override;

    private:
        std::function<std::string(JsonObject&)> _generateResult;

        std::string generatePage(JsonObject& requestData) override;
    };
}