#include "cppflask/html/nodes/MacroNode.h"

#include "cppflask/JsonObject.h"

namespace cppflask::html::nodes {
MacroNode::MacroNode(std::string body, std::vector<std::string> arguments, std::vector<std::string> values) :
    BaseNode("Macro", ""),
    _body{std::move(body)},
    _arguments{std::move(arguments)},
    _values{std::move(values)} {}

MacroNode::~MacroNode() = default;

std::string MacroNode::render(JsonObject& data) const {
    
    return "TODO";
}
}