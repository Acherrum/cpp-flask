#include "cppflask/html/nodes/PipedVariableNode.h"

#include <iostream>
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

    for (const auto& pipeMethod : _pipeMethods) {
        auto var = data.get(_html);
        auto result = JsonObject{FilterRegistry::getFilter(pipeMethod)(var)};
        if (result.hasMember("error")) {
            data.set(_html, result.getValueAsString("input"));
        } else {
            data.set(_html, result);
        }
    }
    return data.getValueAsString(_html);
}
}
