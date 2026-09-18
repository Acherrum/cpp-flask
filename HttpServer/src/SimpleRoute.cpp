#include "cppflask/SimpleRoute.h"

namespace cppflask {
    SimpleRoute::SimpleRoute(const std::string& name,
                std::function<std::string(JsonObject&)> routeResult,
                RouteType type,
                const std::string& contentType) :
        Route(name, type, contentType),
        _generateResult{std::move(routeResult)} {}

    SimpleRoute::~SimpleRoute() = default;

    std::string SimpleRoute::generatePage(JsonObject &requestData) {

        return _generateResult(requestData);
    }
}
