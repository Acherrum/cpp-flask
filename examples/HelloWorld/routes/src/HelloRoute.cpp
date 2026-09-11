#include "HelloRoute.h"

#include "PingPongRoute.h"


HelloRoute::HelloRoute() : cppflask::
    Route{"hello", cppflask::RouteType::POST},
    _html{cppflask::html::HtmlBuilder::fromFile("../www/hello.html")} {

    addRoute(std::make_unique<PingPongRoute>());
}

HelloRoute::~HelloRoute() = default;

std::string HelloRoute::generatePage(cppflask::JsonObject & requestData) {

    return _html.buildWithData(requestData);
}


