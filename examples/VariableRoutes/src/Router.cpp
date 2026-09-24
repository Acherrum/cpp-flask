#include "Router.h"

#include "cppflask/html/HtmlBuilder.h"
#include "cppflask/SimpleRoute.h"

using cppflask::SimpleRoute;
using cppflask::JsonObject;
using cppflask::html::HtmlBuilder;

namespace {
    const HtmlBuilder homeBuilder{HtmlBuilder::fromFile("www/home.html")};
    const HtmlBuilder helloBuilder{HtmlBuilder::fromFile("www/hello.html")};
}

Router::Router() : cppflask::Router{} {

    addRoute(std::make_unique<SimpleRoute>("",
        [&](JsonObject&) {
            return homeBuilder.build();
        }));

    addRoute(std::make_unique<SimpleRoute>("hello/:name",
        [&](JsonObject& request) {
            return helloBuilder.buildWithData(request);
        }));
}

Router::~Router() = default;

