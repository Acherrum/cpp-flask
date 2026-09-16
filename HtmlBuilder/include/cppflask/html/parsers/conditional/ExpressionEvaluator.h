#pragma once

#include <string>

namespace cppflask {
class JsonObject;

namespace html::parsers::conditional {
enum class LogicOperator {
    None, And, Or
};

class ExpressionEvaluator {
public:
    explicit ExpressionEvaluator(const std::string& multiExpression);

    [[nodiscard]] bool evaluate(const JsonObject& data) const;

private:
    struct MultiExpression {
        std::string left;
        LogicOperator logic;
        std::string right;
    };
    MultiExpression _expression;

    [[nodiscard]] MultiExpression parseExpression(const std::string& multiExpression) const;
};
}

}