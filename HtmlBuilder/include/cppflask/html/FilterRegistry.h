#pragma once

#include <unordered_map>
#include <string>
#include <functional>

namespace cppflask {
class JsonObject;

namespace html {
struct HtmlCommand;

    class FilterRegistry {
    public:
        static bool registerFilter(const std::string& name, std::function<std::string(JsonObject&)> method);

        static std::function<std::string(JsonObject&)> getFilter(const std::string& name);

        FilterRegistry(const FilterRegistry&) = delete;
        FilterRegistry& operator=(const FilterRegistry&) = delete;
        FilterRegistry(FilterRegistry&&) = delete;
        FilterRegistry& operator=(FilterRegistry&&) = delete;
    private:
        static FilterRegistry& get();

        std::function<std::string(JsonObject&)> _stub;
        std::unordered_map<std::string, std::function<std::string(JsonObject&)>> _registry{};

        FilterRegistry();
    };
}
}