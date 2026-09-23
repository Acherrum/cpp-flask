#include "Router.h"

#include "cppflask/SimpleRoute.h"

#include "HomeRoute.h"
#include "HelloRoute.h"

Router::Router() : cppflask::Router{} {

    /**
     * This is the RestAPI, in this case we simply have 2 routes.
     */
    addRoute(std::make_unique<HomeRoute>());
    addRoute(std::make_unique<HelloRoute>());

    // using a simple route to create a stop function from the webapp.
    addRoute(std::make_unique<cppflask::SimpleRoute>("stop",
        [&](cppflask::JsonObject&) -> std::string {
            stopServer();
            return "Stopped the server";
        }));
}

Router::~Router() = default;
