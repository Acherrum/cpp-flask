#pragma once

#include "cppflask/IRouter.h"

/**
 * The Router class is the entry point to the webapps RestAPI.
 */
class Router : public cppflask::IRouter {
public:
    Router();
    ~Router() override;

    const std::vector<std::unique_ptr<cppflask::IRoute>>& getRoutes() const;

private:
    std::vector<std::unique_ptr<cppflask::IRoute>> _routes;
};
