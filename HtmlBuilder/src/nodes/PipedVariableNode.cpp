#include "cppflask/html/nodes/PipedVariableNode.h"

#include <chrono>
#include <ranges>

#include "cppflask/html/FilterRegistry.h"
#include "cppflask/JsonObject.h"

namespace cppflask::html::nodes {
PipedVariableNode::PipedVariableNode(const std::string& var, const std::string& pipeExpression) :
    BaseNode("PipedVariable", var),
    _pipeMethods{pipeExpression | std::views::split('|') | std::ranges::to<std::vector<std::string>>()} {
}

PipedVariableNode::~PipedVariableNode() = default;

std::string PipedVariableNode::render(JsonObject &data) const {

    auto timestamp = std::chrono::system_clock::now().time_since_epoch().count();
    auto jsonKey = _html + "_" + std::to_string(timestamp);
    data.set(jsonKey, data.get(_html));
    for (const auto& pipeMethod : _pipeMethods) {
        auto var = data.get(jsonKey);
        auto result = JsonObject{FilterRegistry::getFilter(pipeMethod)(var)};
        if (result.hasMember("error")) {
            data.set(jsonKey, result.getValueAsString("input"));
        } else {
            data.set(jsonKey, result);
        }
    }
    return data.getValueAsString(jsonKey);
}
}
