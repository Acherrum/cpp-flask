#include "Router.h"

#include "cppflask/html/HtmlBuilder.h"
#include "cppflask/SimpleRoute.h"
#include "cppflask/html/StringHelper.h"

using cppflask::SimpleRoute;
using cppflask::JsonObject;
using cppflask::html::HtmlBuilder;

namespace {
    const HtmlBuilder homeBuilder{HtmlBuilder::fromFile("www/home.html")};
    const HtmlBuilder helloBuilder{HtmlBuilder::fromFile("www/hello.html")};
}

Router::Router() {

    _routes.emplace_back(std::make_unique<SimpleRoute>("",
        [&](JsonObject&) {
            return homeBuilder.build();
        }));

    _routes.emplace_back(std::make_unique<SimpleRoute>("hello",
        [&](JsonObject& request) {
            return helloBuilder.buildWithData(request);
        },
        cppflask::RouteType::POST));

    _routes.emplace_back(std::make_unique<SimpleRoute>("hello/ping",
        [&](JsonObject& request) {
            auto result = std::string{};
            auto curVal = request.getValue("get/cur", std::string{});
            cppflask::html::stripAll(curVal);
            try {
                auto newVal = std::stol(curVal);
                if (newVal % 3 == 0) result += "<b>Ping</b>";
                if (newVal % 5 == 0) result += "<u>Pong</u>";
                if (result.empty()) result = curVal;
            } catch (...) {
                auto actual = std::stol(request.getValue("get/val", std::string{}));
                if (actual % 3 == 0) result += "<b>";
                if (actual % 5 == 0) result += "<u>";
                result += std::to_string(actual);
                if (actual % 5 == 0) result += "</u>";
                if (actual % 3 == 0) result += "</b>";

            }

            auto object = cppflask::JsonObject{
                "{\"response\":\""+ result +"\"}"
            };
            return object.toString();
        },
        cppflask::RouteType::GET,
        "application/json"));
}

Router::~Router() = default;

const std::vector<std::unique_ptr<cppflask::IRoute>> &Router::getRoutes() const {

    return _routes;
}
