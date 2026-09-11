#include "cppflask/html/parsers/loop/ExpressionEvaluator.h"

#include "cppflask/html/StringHelper.h"

namespace cppflask::html::parsers::loop {

LoopSettings ExpressionEvaluator::evaluate(const std::string& expression)
{
    auto settings = LoopSettings{};
    auto toPos = expression.find(" TO ");
    if (toPos != std::string::npos) {
        auto assignmentPos = expression.find('=');
        settings.variable = expression.substr(0, assignmentPos);
        stripAll(settings.variable);

        auto startPos = assignmentPos + 1;
        settings.start = expression.substr(startPos, toPos - startPos);
        stripAll(settings.start);
        settings.end = expression.substr(toPos + 4);
        stripAll(settings.end);
    } else {
        auto inPos = expression.find(" IN ");
        if (inPos == std::string::npos) {
            return {};
        }
        settings.variable = expression.substr(0, inPos);
        stripAll(settings.variable);
        settings.dataVar = expression.substr(inPos + 4);
        if (settings.dataVar.at(0) == '$') {
            settings.dataVar = settings.dataVar.substr(1);
        }
    }

    return settings;
}

}
