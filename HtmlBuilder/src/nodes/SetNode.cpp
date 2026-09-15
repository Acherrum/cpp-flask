#include "cppflask/html/nodes/SetNode.h"

#include <algorithm>
#include <cmath>
#include <string>

#include "cppflask/JsonObject.h"

#include "cppflask/html/parsers/simple/CommandVariablesParser.h"


namespace {

    struct Number {
        bool isFloatingPoint{false};
        bool isNegative{false};
        std::string value;

        Number operator+(const Number& right) {
            if (isFloatingPoint || right.isFloatingPoint) {
                auto numericResult = std::stod(value) + std::stod(right.value);
                return {
                    true,
                    numericResult < 0.0,
                    std::to_string(numericResult)
                };
            } else {
                auto numericResult = std::stoll(value) + std::stoll(right.value);
                return {
                    false,
                    numericResult < 0,
                    std::to_string(numericResult)
                };
            }
        }

        Number operator-(const Number& right) {
            if (isFloatingPoint || right.isFloatingPoint) {
                auto numericResult = std::stod(value) - std::stod(right.value);
                return {
                    true,
                    numericResult < 0.0,
                    std::to_string(numericResult)
                };
            } else {
                auto numericResult = std::stoll(value) - std::stoll(right.value);
                return {
                    false,
                    numericResult < 0,
                    std::to_string(numericResult)
                };
            }
        }

        Number operator*(const Number& right) {
            if (isFloatingPoint || right.isFloatingPoint) {
                auto numericResult = std::stod(value) * std::stod(right.value);
                return {
                    true,
                    numericResult < 0.0,
                    std::to_string(numericResult)
                };
            } else {
                auto numericResult = std::stoll(value) * std::stoll(right.value);
                return {
                    false,
                    numericResult < 0,
                    std::to_string(numericResult)
                };
            }
        }

        Number operator/(const Number& right) {
            auto numericResult = std::stod(value) / std::stod(right.value);
            return {
                true,
                numericResult < 0.0,
                std::to_string(numericResult)
            };
        }

        Number operator%(const Number& right) {
            if (isFloatingPoint || right.isFloatingPoint) {
                auto numericResult = fmod(std::stod(value),std::stod(right.value));
                return {
                    true,
                    numericResult < 0.0,
                    std::to_string(numericResult)
                };
            } else {
                auto numericResult = std::stoll(value) % std::stoll(right.value);
                return {
                    false,
                    numericResult < 0,
                    std::to_string(numericResult)
                };
            }
        }
    };

    struct Operator {
        char op;
        std::function<Number(Number, Number)> operation;
    };

    const std::vector<Operator>& getSupportedOperators() {
        static const std::vector<Operator> operators{
            {'+', [](Number left, Number right) { return left + right; }},
            {'-', [](Number left, Number right) { return left - right; }},
            {'*', [](Number left, Number right) { return left * right; }},
            {'/', [](Number left, Number right) { return left / right; }},
            {'%', [](Number left, Number right) { return left % right; }}
        };
        return operators;
    }

    std::string parseStringLiteral(const std::string& expr) {

        char quote = expr.front();

        size_t endPos = expr.find(quote, 1);
        if (endPos == std::string::npos) {
            return {};
        }

        return expr.substr(1, endPos - 1);
    }

    Number parseNumericExpression(const std::string& expr);

    std::string resolveBraces(const std::string& expr) {
        auto result = expr;
        auto subExprStart = result.find('(');
        while (subExprStart != std::string::npos) {
            auto subExprEnd = result.find(')', subExprStart + 1);
            auto number = parseNumericExpression(result.substr(subExprStart + 1, subExprEnd));
            result.replace(subExprStart, subExprEnd + 1, number.value);
            subExprStart = result.find('(');
        }
        return result;
    }

    bool isOperator(char c) {
        return c == '+' || c == '-' || c == '*' || c == '/' || c == '%';
    }

    Number parseTerm(const std::string& expr) {
        size_t operatorPos = expr.find_last_of("*/%");

        if (operatorPos != std::string::npos) {
            char op = expr[operatorPos];
            auto left = parseTerm(expr.substr(0, operatorPos));
            auto right = parseNumericExpression(expr.substr(operatorPos + 1));

            const auto& operators = getSupportedOperators();
            auto operIt = std::find_if(operators.begin(), operators.end(), [&](const auto& o) { return o.op == op; });

            return operIt->operation(left, right);
        }

        if (!expr.empty() && expr[0] == '-') {
            auto positiveValue = parseNumericExpression(expr.substr(1));
            positiveValue.isNegative = true;
            positiveValue.value = "-" + positiveValue.value;
            return positiveValue;
        }

        return {expr.find(".") != std::string::npos, false, expr};
    }

    Number parseNumericExpression(const std::string& expr) {
        for (size_t i = expr.length(); i > 0; --i) {
            size_t idx = i - 1;
            char c = expr[idx];

            if (c == '+' || c == '-') {
                if (c == '-' && (idx == 0 || isOperator(expr[idx - 1]))) {
                    continue;
                }

                auto left = parseNumericExpression(expr.substr(0, idx));
                auto right = parseTerm(expr.substr(idx + 1));

                const auto& operators = getSupportedOperators();
                auto operIt = std::find_if(operators.begin(), operators.end(), [&](const auto& o) { return o.op == c; });

                return operIt->operation(left, right);
            }
        }

        return parseTerm(expr);
    }
}

namespace cppflask::html::nodes {

SetNode::SetNode(std::string key, std::string expression) : BaseNode("Set",""), _key{std::move(key)}, _expression{std::move(expression)} {}

SetNode::~SetNode() = default;

std::string SetNode::render(JsonObject& data){

    auto tempExpr = _expression;
    parsers::CommandVariablesParser{tempExpr}.parse(data);

    if (!tempExpr.empty() && (tempExpr.front() == '"' || tempExpr.front() == '\'')) {
        data.set(_key, parseStringLiteral(tempExpr));
    } else {
        auto exprWithoutBraces = resolveBraces(tempExpr);
        auto result = parseNumericExpression(exprWithoutBraces);
        if (result.isFloatingPoint) {
            data.set(_key, std::stod(result.value));
        } else if (result.isNegative) {
            data.set(_key, std::stol(result.value));
        } else {
            data.set(_key, std::stoul(result.value));
        }
    }
    return {};
}
}
