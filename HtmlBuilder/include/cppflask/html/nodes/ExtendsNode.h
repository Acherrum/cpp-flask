#pragma once

#include "cppflask/html/HtmlBuilder.h"
#include "cppflask/html/nodes/BaseNode.h"

namespace cppflask::html::nodes {

    class ExtendsNode : public BaseNode {
    public:
        explicit ExtendsNode(const std::string& html);

        ~ExtendsNode() override;
        std::string render(JsonObject& data) const override;
    private:
        HtmlBuilder _builder;
    };

}