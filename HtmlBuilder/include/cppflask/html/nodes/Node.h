#pragma once

#include <string>

namespace cppflask {
class JsonObject;

namespace html::nodes {
class Node {
public:
    virtual ~Node() = default;
    virtual std::string render(JsonObject& data) const = 0;

    [[nodiscard]] virtual const std::string& getType() const = 0;
};
}
}