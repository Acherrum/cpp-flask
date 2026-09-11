#pragma once

#include <string>
#include <optional>

namespace cppflask {
class JsonObject;

namespace html::parsers::conditional {
class ExpressionEvaluator {
public:
    explicit ExpressionEvaluator(const std::string& multiExpression);

    bool evaluate(const JsonObject& data) const;

private:
    struct MultiExpression {
        std::string left;
        std::string logic;
        std::string right;
    };
    MultiExpression _expression;

    MultiExpression parseExpression(const std::string& multiExpression) const;
};
}

}