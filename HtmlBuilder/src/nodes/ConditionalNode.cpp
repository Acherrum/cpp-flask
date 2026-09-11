#include "cppflask/html/nodes/ConditionalNode.h"

#include "cppflask/html/HtmlBuilder.h"
#include "cppflask/JsonObject.h"

namespace cppflask::html::nodes {

ConditionalNode::ConditionalNode(parsers::conditional::ExpressionEvaluator expression,
        std::unique_ptr<HtmlBuilder> trueBranch,
        std::unique_ptr<HtmlBuilder> falseBranch) :
    BaseNode("conditional",""),
    _expression(std::move(expression)),
    _trueBranch(std::move(trueBranch)),
    _falseBranch(std::move(falseBranch)) {
}

ConditionalNode::~ConditionalNode() = default;

std::string ConditionalNode::render(JsonObject& data)
{
    if (_expression.evaluate(data)) {
        return _trueBranch->buildWithData(data);
    }

    if (_falseBranch) {
        return _falseBranch->buildWithData(data);
    }

    return {};
}

}
