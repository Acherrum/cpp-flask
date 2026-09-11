#pragma once

#include "cppflask/IRoute.h"


namespace cppflask {
    class Route : public IRoute {
    public:
        Route(const std::string& name, RouteType type = RouteType::GET, const std::string& contentType = "text/html");
        ~Route() override;

        RouteType getType() const override;
        const std::string& getName() const override;
        const std::string& getContentType() const override;

        const std::unordered_map<std::string, std::string>& getHeaders() const override;
        const std::vector<std::unique_ptr<IRoute>>& getRoutes() const override;

    protected:
        void setHeader(const std::string& key, const std::string& value);
        void addRoute(std::unique_ptr<IRoute> newRoute);

    private:
        RouteType _type;
        std::string _name;
        std::string _contentType;

        std::unordered_map<std::string, std::string> _headers;
        std::vector<std::unique_ptr<IRoute>> _routes;
    };
}