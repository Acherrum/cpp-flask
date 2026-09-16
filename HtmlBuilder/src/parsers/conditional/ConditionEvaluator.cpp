#include "cppflask/html/parsers/conditional/ConditionEvaluator.h"

#include <vector>
#include <unordered_map>

#include "cppflask/html/StringHelper.h"

#include "cppflask/JsonObject.h"


namespace cppflask::html::parsers::conditional {
ConditionEvaluator::ConditionEvaluator(const std::string& expression) : _expression{parseExpression(expression)} {}

bool ConditionEvaluator::evaluate(const JsonObject& data) const {

    if (_expression.comp == Comparator::NONE) {
        auto expression = _expression.left;
        auto NOT = false;
        if (expression.at(0) == '!') {
            NOT = true;
            expression = expression.substr(1);
        }
        if (expression.at(0) == '$') {
            expression = expression.substr(1);
        }
        auto result = data.getValue(expression, false);
        return (NOT ? !result : result);
    }

    auto literalCompare = (_expression.left.at(0) != '$' || _expression.right.at(0) != '$');

    auto left = _expression.left;
    auto right = _expression.right;
    if (literalCompare) {
        if (left.at(0) == '$') {
            left = data.getValueAsString(left.substr(1));
        } else if (left.at(0) == '"' || left.at(0) == '\'') {
            left = left.substr(1, left.length()-2);
        }
        if (right.at(0) == '$') {
            right = data.getValueAsString(right.substr(1));
        } else if (right.at(0) == '"' || right.at(0) == '\'') {
            right = right.substr(1, right.length()-2);
        }

        try {
            return eval(std::stod(left), _expression.comp, std::stod(right));
        } catch (...) {
            return eval(left, _expression.comp, right);
        }
    }
    return data.compareKeys(left.substr(1), right.substr(1),
                            [&](double a, double b) { return eval(a, _expression.comp, b); },
                            [&](const std::string& a, const std::string& b) { return eval(a, _expression.comp, b); },
                            [&](bool a, bool b) { return eval(a, _expression.comp, b); });
}

ConditionEvaluator::Expression ConditionEvaluator::parseExpression(const std::string& expression) {

    for (const auto& compPair : getComparatorMap()) {
        auto comp = compPair.first;
        auto pos = expression.find(comp);
        if (pos != std::string::npos) {
            auto left = expression.substr(0,pos);
            auto right = expression.substr(pos + comp.length());
            stripAll(left);
            stripAll(right);
            return {
                .left = left,
                .comp = comparatorFromString(comp),
                .right = right
            };
        }
    }
    return {.left = expression,.comp = Comparator::NONE, .right = ""};
}

ConditionEvaluator::Comparator ConditionEvaluator::comparatorFromString(const std::string& comp) {

    auto result = getComparatorMap().find(comp);
    if (result == getComparatorMap().end()) {
        return Comparator::NONE;
    }
    return result->second;
}

const std::unordered_map<std::string, ConditionEvaluator::Comparator>& ConditionEvaluator::getComparatorMap() {
    static const std::unordered_map<std::string, ConditionEvaluator::Comparator> compareMap{
        {"==", Comparator::EQ},
        {"!=", Comparator::NEQ},
        {">=", Comparator::GTE},
        {"<=", Comparator::LTE},
        {">", Comparator::GT},
        {"<", Comparator::LT}
    };
    return compareMap;
}
}
