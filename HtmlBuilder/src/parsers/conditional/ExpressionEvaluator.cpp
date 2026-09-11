#include "cppflask/html/parsers/conditional/ExpressionEvaluator.h"

#include "cppflask/html/parsers/conditional/ConditionEvaluator.h"
#include "cppflask/html/StringHelper.h"

namespace cppflask::html::parsers::conditional {
ExpressionEvaluator::ExpressionEvaluator(const std::string &multiExpression) :
    _expression{parseExpression(multiExpression)}
{
}

bool ExpressionEvaluator::evaluate(const JsonObject &data) const
{
    auto result = ConditionEvaluator{_expression.left}.evaluate(data);
    if (_expression.logic.empty()) {
        return result;
    } else if (_expression.logic == "AND") {
        return result && ExpressionEvaluator{_expression.right}.evaluate(data);
    } else if (_expression.logic == "OR") {
        return result || ExpressionEvaluator{_expression.right}.evaluate(data);
    }
    return false;
}

ExpressionEvaluator::MultiExpression ExpressionEvaluator::parseExpression(const std::string &multiExpression) const
{

    auto startPos = multiExpression.find("(") + 1;
    auto endPos = multiExpression.rfind(")");
    auto fullExpression = multiExpression.substr(startPos, endPos-startPos);
    auto andPos = fullExpression.find("AND");
    auto orPos = fullExpression.find("OR");
    if (andPos == std::string::npos && orPos == std::string::npos) {
        return { fullExpression, "", ""};
    } else if (andPos < orPos) {
        auto left = strip(fullExpression.substr(0, andPos));
        auto right = strip(fullExpression.substr(andPos + 3));
        return {left, "AND", right};
    } else {
        auto left = strip(fullExpression.substr(0, orPos));
        auto right = strip(fullExpression.substr(orPos + 3));
        return {left, "OR", right};
    }
}

}
