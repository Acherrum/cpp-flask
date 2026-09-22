#include "cppflask/html/FilterRegistry.h"

#include "cppflask/JsonObject.h"

namespace cppflask::html {

FilterRegistry::FilterRegistry() : _stub{[](const JsonObject& data) { return data.toString(); }} {}


bool FilterRegistry::registerFilter(const std::string &name,
    std::function<std::string(JsonObject&)> method) {

    get()._registry[name] = std::move(method);
    return true;
}

std::function<std::string(JsonObject&)> FilterRegistry::getFilter(const std::string &name) {

    auto& instance = get();
    auto iter = instance._registry.find(name);
    if (iter == instance._registry.end()) {
        return instance._stub;
    }
    return iter->second;
}

FilterRegistry& FilterRegistry::get() {

    static FilterRegistry _instance{};
    return _instance;
}
}
