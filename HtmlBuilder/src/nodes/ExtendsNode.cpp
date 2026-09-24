#include "cppflask/html/nodes/ExtendsNode.h"


namespace cppflask::html::nodes {
    ExtendsNode::ExtendsNode(const std::string& html) :
        BaseNode{"extends", ""},
        _builder{HtmlBuilder::fromText(html)} {
    }

    ExtendsNode::~ExtendsNode() = default;

    std::string ExtendsNode::render(JsonObject& data) const {

        return _builder.buildWithData(data);
    }
}