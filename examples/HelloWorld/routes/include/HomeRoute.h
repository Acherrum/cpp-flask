#include "cppflask/Route.h"

#include "cppflask/html/HtmlBuilder.h"

class HomeRoute : public cppflask::Route {
public:
    HomeRoute();
    ~HomeRoute() override;

private:
    cppflask::html::HtmlBuilder _html;

    std::string generatePage(cppflask::JsonObject& requestData) override;
};