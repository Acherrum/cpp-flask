#include <utility>

#include "cppflask/html/parsers/conditional/ConditionalsParser.h"

#include "cppflask/html/parsers/conditional/ExpressionEvaluator.h"
#include "cppflask/html/nodes/HtmlNode.h"
#include "cppflask/html/nodes/ConditionalNode.h"
#include "cppflask/html/HtmlCommand.h"

#include "cppflask/JsonObject.h"
#include "cppflask/html/HtmlBuilder.h"

namespace {
constexpr std::string START_TEXT{"{% IF "};
constexpr std::string START_ELSE_TEXT{"{% ELSE "};
constexpr std::string ELSE_TEXT{"{% ELSE %}"};
constexpr std::string END_TEXT{"{% END_IF %}"};
}

namespace cppflask::html::parsers::conditional {
ConditionalsParser::ConditionalsParser(const std::string& html) : _html{html} {}

std::pair<std::size_t, std::unique_ptr<nodes::BaseNode>> ConditionalsParser::parse(const HtmlCommand& cmd)
{
    std::unique_ptr<HtmlBuilder> nested{nullptr};
    auto elseCase = std::unique_ptr<HtmlBuilder>();

    auto isEndCommand = cmd.cmd.find(END_TEXT) != std::string::npos;
    std::size_t beginStatement = 0;
    std::size_t endStatement = 0;
    if (!isEndCommand) {
        beginStatement = cmd.cmd.find("(");
        endStatement = cmd.cmd.rfind(")");
        if (beginStatement == std::string::npos || endStatement == std::string::npos) {
            return { cmd.endPos, std::make_unique<nodes::HtmlNode>("<b>Parse error:</b> Missing brackets.<br />Occured here: " + cmd.cmd) };
        }
    }
    auto [endPos, endCommand] = findEnd(cmd.endPos);
    if (endPos == std::string::npos) {
        return { cmd.endPos, std::make_unique<nodes::HtmlNode>("<b>Parse error:</b> Missing END_IF command.<br />Occured here: " + cmd.cmd) };
    }
    auto [elsePos, elseCommand] = findEnd(cmd.endPos, START_ELSE_TEXT);
    auto endOfParsedData = endPos + endCommand.length();
    if (elsePos < endPos) {
        nested = HtmlBuilder::uniqueFromText(_html.substr(cmd.endPos, elsePos - cmd.endPos));
        if (elseCommand == ELSE_TEXT) {
            elseCase = HtmlBuilder::uniqueFromText(_html.substr(elsePos + elseCommand.length(), endPos - (elsePos + elseCommand.length())));
        } else {
            elseCase = HtmlBuilder::uniqueFromText(_html.substr(elsePos, endOfParsedData - elsePos));
        }
    } else {
        nested = HtmlBuilder::uniqueFromText(_html.substr(cmd.endPos, endPos - cmd.endPos));
    }
    return { endOfParsedData, std::make_unique<nodes::ConditionalNode>(
        ExpressionEvaluator{cmd.cmd.substr(beginStatement, ++endStatement-beginStatement)},
        std::move(nested),
        std::move(elseCase))
    };
}


std::pair<std::size_t, std::string> ConditionalsParser::findEnd(std::size_t pos, const std::string& endText) const {
    auto ifCount = 1;
    auto nextIfPos = _html.find(START_TEXT, pos);
    auto endIfPos = _html.find(endText, pos);
    if (endIfPos == std::string::npos) {
        return {std::string::npos, ""};
    }
    while (ifCount > 0) {
        if (nextIfPos > endIfPos) {
            ifCount--;
            if (ifCount == 0) {
                return {endIfPos, _html.substr(endIfPos, _html.find("%}", endIfPos) - endIfPos + 2)};
            }
            endIfPos = _html.find(endText, endIfPos + endText.length());
        } else {
            ifCount++;
            nextIfPos = _html.find(START_TEXT, nextIfPos + 6);
        }
        if (endIfPos == std::string::npos) {
            break;
        }
    }
    return {std::string::npos, ""};
}

}
