#pragma once

#include "cppflask/IRouter.h"

namespace cppflask {
    class Router : public virtual IRouter {
    public:
        Router();
        ~Router() override;

        void setStopCommand(std::function<void()>) override;
        [[nodiscard]] const std::vector<std::unique_ptr<IRoute>>& getRoutes() const override;

    protected:
        void addRoute(std::unique_ptr<IRoute> route);
        void stopServer() const;

    private:
        std::vector<std::unique_ptr<IRoute>> _routes{};
        std::function<void()> _stopCommand{};
    };
}