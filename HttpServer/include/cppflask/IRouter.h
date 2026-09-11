#pragma once

#include <memory>
#include <vector>

namespace cppflask {
    class IRoute;

    class IRouter {
    public:
        virtual ~IRouter() = default;
        [[nodiscard]] virtual const std::vector<std::unique_ptr<IRoute>>& getRoutes() const = 0;
    };
}
