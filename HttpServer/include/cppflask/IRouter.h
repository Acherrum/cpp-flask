#pragma once

#include <memory>
#include <vector>
#include <functional>

namespace cppflask {
    class IRoute;

    class IRouter {
    public:
        virtual ~IRouter() = default;
        virtual void setStopCommand(std::function<void()> serverStopCommand) = 0;
        [[nodiscard]] virtual const std::vector<std::unique_ptr<IRoute>>& getRoutes() const = 0;
    };
}
