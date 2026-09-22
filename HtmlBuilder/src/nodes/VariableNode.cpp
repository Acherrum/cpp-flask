#include "cppflask/html/nodes/VariableNode.h"

#include "cppflask/JsonObject.h"

namespace cppflask::html::nodes {
VariableNode::VariableNode(const std::string& html) : BaseNode{"Variable", html} {


}
VariableNode::~VariableNode() = default;

std::string VariableNode::render(JsonObject& data) const
{
    return data.getValueAsString(_html);
}

}