#include "cppflask/Router.h"

#include "cppflask/IRoute.h"

namespace cppflask {
    Router::Router() = default;

    Router::~Router() = default;

    void Router::setStopCommand(std::function<void()> command) {

        _stopCommand = std::move(command);
    }

    const std::vector<std::unique_ptr<IRoute>> &Router::getRoutes() const {

        return _routes;
    }

    void Router::addRoute(std::unique_ptr<IRoute> route) {

        _routes.emplace_back(std::move(route));
    }

    void Router::stopServer() const {

        _stopCommand();
    }
}