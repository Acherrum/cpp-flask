#pragma once

#include "cppflask/IRouter.h"

#include <string>
#include <unordered_map>

#include "cppflask/JsonObject.h"

namespace cppflask {
    enum class RouteType {
        GET, POST
    };

    class IRoute : public IRouter {
    public:
        ~IRoute() override = default;

        [[nodiscard]] virtual RouteType getType() const = 0;
        [[nodiscard]] virtual const std::string& getName() const = 0;
        [[nodiscard]] virtual const std::string& getContentType() const = 0;
        [[nodiscard]] virtual const std::unordered_map<std::string, std::string>& getHeaders() const = 0;

        virtual std::string generatePage(JsonObject& requestData) = 0;
    };
}
