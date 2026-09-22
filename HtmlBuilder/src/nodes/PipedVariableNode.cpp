#include "cppflask/html/nodes/PipedVariableNode.h"

#include "cppflask/html/FilterRegistry.h"
#include "cppflask/JsonObject.h"

namespace cppflask::html::nodes {
PipedVariableNode::PipedVariableNode(const std::string& var, const std::string& pipeMethod) :
    BaseNode("PipedVariable", var),
    _pipeMethod(FilterRegistry::getFilter(pipeMethod)) {
}

PipedVariableNode::~PipedVariableNode() = default;

std::string PipedVariableNode::render(JsonObject &data) {

    auto var = data.get(_html);
    return _pipeMethod(var);
}
}
