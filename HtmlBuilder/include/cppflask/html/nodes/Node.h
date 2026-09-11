#pragma once

#include <string>

namespace cppflask {
class JsonObject;

namespace html::nodes {
class Node {
public:
    virtual ~Node() = default;
    virtual std::string render(JsonObject& data) = 0;

    virtual const std::string& getType() const = 0;
};
}
}