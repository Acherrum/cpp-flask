#pragma once

#include "cppflask/Route.h"

class PingPongRoute : public cppflask::Route {
public:
    PingPongRoute();
    ~PingPongRoute() override;

private:
    std::string generatePage(cppflask::JsonObject& requestData) override;
};