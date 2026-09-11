#include "PingPongRoute.h"

#include "cppflask/html/StringHelper.h"

PingPongRoute::PingPongRoute() :
    cppflask::Route{"ping", cppflask::RouteType::GET, "application/json"} {}

PingPongRoute::~PingPongRoute() = default;

std::string PingPongRoute::generatePage(cppflask::JsonObject& data) {

    auto result = std::string{};
    auto curVal = data.getValue("get/cur", std::string{});
    cppflask::html::stripAll(curVal);
    try {
        auto newVal = std::stol(curVal);
        if (newVal % 3 == 0) result += "<b>Ping</b>";
        if (newVal % 5 == 0) result += "<u>Pong</u>";
        if (result.empty()) result = curVal;
    } catch (...) {
        auto actual = std::stol(data.getValue("get/val", std::string{}));
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

}


