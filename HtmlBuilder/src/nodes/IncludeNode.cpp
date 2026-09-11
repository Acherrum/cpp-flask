#include "cppflask/html/nodes/IncludeNode.h"

#include "cppflask/JsonObject.h"
#include "cppflask/html/HtmlBuilder.h"

namespace cppflask::html::nodes {

    IncludeNode::IncludeNode(const std::string& variable) :
        BaseNode("Include", ""),
        _variable{variable} {}

    IncludeNode::~IncludeNode() = default;

    std::string IncludeNode::render(JsonObject& data) {

        return HtmlBuilder::fromText(data.getValueAsString(_variable))
            .buildWithData(data);
    }

} // namespace cppflask::html::nodes
