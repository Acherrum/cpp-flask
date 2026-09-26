#include "Router.h"

#include "cppflask/html/HtmlBuilder.h"
#include "cppflask/SimpleRoute.h"

using cppflask::SimpleRoute;
using cppflask::JsonObject;
using cppflask::html::HtmlBuilder;

namespace {
    const HtmlBuilder homeBuilder{HtmlBuilder::fromFile("www/home.html")};
}

Router::Router() : cppflask::Router{} {
    addRoute(std::make_unique<SimpleRoute>("",
        [&](JsonObject& req) {
            req.set("params/rows", 0L);
            return homeBuilder.buildWithData(req);
        }));
    addRoute(std::make_unique<SimpleRoute>("show/:rows",
        [&](JsonObject& req) {
            return homeBuilder.buildWithData(req);
        }));
}

Router::~Router() = default;

