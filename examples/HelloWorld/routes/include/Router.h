#pragma once

#include "cppflask/Router.h"

/**
 * The Router class is the entry point to the webapps RestAPI.
 */
class Router : public cppflask::Router {
public:
    Router();
    ~Router() override;
};
