#pragma once

#include "cppflask/IRouter.h"

#include <string>
#include <unordered_map>

#include "cppflask/JsonObject.h"

namespace cppflask {
    enum class RouteType {
        GET, POST
    };

    inline std::string toString(RouteType val) {
        return val == RouteType::GET ? "GET" : "POST";
    }
    
    class IRoute : public virtual IRouter {
    public:
        ~IRoute() override = default;

        [[nodiscard]] virtual RouteType getType() const = 0;
        [[nodiscard]] virtual const std::string& getName() const = 0;
        [[nodiscard]] virtual const std::string& getContentType() const = 0;
        [[nodiscard]] virtual const std::unordered_multimap<std::string, std::string>& getHeaders() const = 0;

        virtual std::string generatePage(JsonObject& requestData) = 0;
    };
}
