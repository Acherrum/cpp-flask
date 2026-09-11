#pragma once

#include <memory>
#include <vector>

#include "cppflask/IRoute.h"

namespace cppflask {
    class IRouter {
    public:
        virtual ~IRouter() = default;
        virtual const std::vector<std::unique_ptr<IRoute>>& getRoutes() const = 0;
    };
}
