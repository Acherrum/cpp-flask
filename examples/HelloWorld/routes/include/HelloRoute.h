#pragma once

#include "cppflask/Route.h"

#include "cppflask/html/HtmlBuilder.h"

class HelloRoute : public cppflask::Route {
public:
    HelloRoute();
    ~HelloRoute() override;

private:
    cppflask::html::HtmlBuilder _html;

    std::string generatePage(cppflask::JsonObject& requestData) override;
};