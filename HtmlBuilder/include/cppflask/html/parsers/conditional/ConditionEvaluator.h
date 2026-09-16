#pragma once

#include <string>
#include <unordered_map>

namespace cppflask {
class JsonObject;

namespace html::parsers::conditional {
class ConditionEvaluator {
public:
    explicit ConditionEvaluator(const std::string& expression);

    [[nodiscard]] bool evaluate(const JsonObject& data) const;

private:
    enum class Comparator {
        NONE, EQ, NEQ, GTE, LTE, GT, LT
    };

    struct Expression {
        std::string left;
        Comparator comp;
        std::string right;
    };

    Expression _expression;

    static Expression parseExpression(const std::string& expression);
    static Comparator comparatorFromString(const std::string& comp);
    static const std::unordered_map<std::string, Comparator>& getComparatorMap();

    template <typename TYPE>
    static bool eval(TYPE left, Comparator compare, TYPE right) {
        switch (compare) {
            case Comparator::EQ: return left == right;
            case Comparator::NEQ: return left != right;
            case Comparator::GTE: return left >= right;
            case Comparator::LTE: return left <= right;
            case Comparator::GT: return left > right;
            case Comparator::LT: return left < right;
            default: return false;
        }
    }
};
}
}