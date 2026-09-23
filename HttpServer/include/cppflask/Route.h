#pragma once

#include "cppflask/IRoute.h"
#include "cppflask/Router.h"


namespace cppflask {
    class Route : public Router, public IRoute {
    public:
        explicit Route(const std::string& name,
            RouteType type = RouteType::GET,
            const std::string& contentType = "text/html");
        ~Route() override;

        RouteType getType() const override;
        const std::string& getName() const override;
        const std::string& getContentType() const override;

        const std::unordered_multimap<std::string, std::string>& getHeaders() const override;

    protected:
        void setHeader(const std::string& key, const std::string& value);
        void setCookie(const std::string& key, const std::string& value, unsigned long lifetime);

    private:
        RouteType _type;
        std::string _name;
        std::string _contentType;

        std::unordered_multimap<std::string, std::string> _headers;
    };
}