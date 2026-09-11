#include "HelloRoute.h"

#include <iostream>

HelloRoute::HelloRoute() : cppflask::
    Route{"hello", cppflask::RouteType::POST},
    _html{cppflask::html::HtmlBuilder::fromFile("../www/hello.html")} {
}

HelloRoute::~HelloRoute() = default;

std::string HelloRoute::generatePage(cppflask::JsonObject & requestData) {

    auto html = _html.buildWithData(requestData);;
    return html;
}


