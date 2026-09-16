#include "cppflask/html/parsers/conditional/ExpressionEvaluator.h"

#include "cppflask/html/parsers/conditional/ConditionEvaluator.h"
#include "cppflask/html/StringHelper.h"

namespace {
    const std::string AND_STR{" AND "};
    const std::string OR_STR{" OR "};
}

namespace cppflask::html::parsers::conditional {
ExpressionEvaluator::ExpressionEvaluator(const std::string &multiExpression) :
    _expression{parseExpression(multiExpression)}
{
}

bool ExpressionEvaluator::evaluate(const JsonObject &data) const
{
    auto result = ConditionEvaluator{_expression.left}.evaluate(data);
    if (_expression.logic == LogicOperator::None) {
        return result;
    } else if (_expression.logic == LogicOperator::And) {
        return result && ExpressionEvaluator{_expression.right}.evaluate(data);
    } else if (_expression.logic == LogicOperator::Or) {
        return result || ExpressionEvaluator{_expression.right}.evaluate(data);
    }
    return false;
}

ExpressionEvaluator::MultiExpression ExpressionEvaluator::parseExpression(const std::string &multiExpression) const
{

    auto startPos = multiExpression.find("(") + 1;
    auto endPos = multiExpression.rfind(")");
    auto fullExpression = multiExpression.substr(startPos, endPos-startPos);
    auto andPos = find(fullExpression, AND_STR);
    auto orPos = find(fullExpression, OR_STR);
    if (andPos == std::string::npos && orPos == std::string::npos) {
        return { fullExpression, LogicOperator::None, ""};
    }
    if (andPos < orPos) {
        auto left = strip(fullExpression.substr(0, andPos));
        auto right = strip(fullExpression.substr(andPos + AND_STR.length()));
        return {left, LogicOperator::And, right};
    }

    auto left = strip(fullExpression.substr(0, orPos));
    auto right = strip(fullExpression.substr(orPos + OR_STR.length()));
    return {left, LogicOperator::Or, right};
}

}
