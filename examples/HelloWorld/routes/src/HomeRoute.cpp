#include "HomeRoute.h"

HomeRoute::HomeRoute() :
    cppflask::Route{""},
    _html{cppflask::html::HtmlBuilder::fromFile("../www/home.html")} {}

HomeRoute::~HomeRoute() = default;

std::string HomeRoute::generatePage(cppflask::JsonObject &) {

    return _html.build();

}


