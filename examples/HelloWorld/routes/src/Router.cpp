#include "Router.h"

#include "HomeRoute.h"
#include "HelloRoute.h"

Router::Router() : _routes{} {

    /**
     * This is the RestAPI, in this case we simply have 2 routes.
     */
    _routes.emplace_back(std::make_unique<HomeRoute>());
    _routes.emplace_back(std::make_unique<HelloRoute>());
}

Router::~Router() = default;

const std::vector<std::unique_ptr<cppflask::IRoute>> &Router::getRoutes() const {

    return _routes;
}
