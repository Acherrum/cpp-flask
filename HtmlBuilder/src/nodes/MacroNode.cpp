#include "cppflask/html/nodes/MacroNode.h"

#include <chrono>

#include "cppflask/html/HtmlBuilder.h"
#include "cppflask/JsonObject.h"

namespace cppflask::html::nodes {
MacroNode::MacroNode(std::string body, const std::vector<std::string>& arguments, const std::vector<std::string>& values) :
    BaseNode("Macro", ""),
    _body{std::move(body)},
    _arguments{} {
    for (auto i = 0; i < arguments.size(); i++) {
        _arguments[arguments.at(i)] = values.at(i);
    }
}

MacroNode::~MacroNode() = default;

std::string MacroNode::render(JsonObject& data) const {

    auto body = _body;
    auto timestamp = std::chrono::system_clock::now().time_since_epoch().count();
    auto jsonKey = "macro_" + std::to_string(timestamp) + "/";
    for (const auto& [argument, value] : _arguments) {
        auto argumentPos = body.find('$' + argument);
        if (value.at(0) == '$') {
            body.replace(argumentPos, argument.length() + 1, value);
        } else {
            body.replace(argumentPos + 1, argument.length(), jsonKey + argument);
            data.set(jsonKey + argument, value);
        }
    }
    return HtmlBuilder::fromText(body).buildWithData(data);
}
}