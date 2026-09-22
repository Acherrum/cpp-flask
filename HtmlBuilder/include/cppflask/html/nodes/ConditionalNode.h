#pragma once

#include "cppflask/html/nodes/BaseNode.h"

#include <string>
#include <memory>

#include "cppflask/html/parsers/conditional/ExpressionEvaluator.h"

namespace cppflask::html {
class HtmlBuilder;

namespace nodes {

class ConditionalNode : public BaseNode {
public:
    ConditionalNode(parsers::conditional::ExpressionEvaluator expression,
        std::unique_ptr<HtmlBuilder> trueBranch,
        std::unique_ptr<HtmlBuilder> falseBranch);

    ~ConditionalNode() override;

    std::string render(JsonObject& data) const override;

private:
    parsers::conditional::ExpressionEvaluator _expression;
    std::unique_ptr<HtmlBuilder> _trueBranch;
    std::unique_ptr<HtmlBuilder> _falseBranch;
};

}

}