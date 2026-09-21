#include "cppflask/html/parsers/loop/LoopParser.h"

#include "cppflask/html/parsers/loop/ExpressionEvaluator.h"
#include "cppflask/html/parsers/loop/LoopSettings.h"
#include "cppflask/html/HtmlCommand.h"
#include "cppflask/JsonObject.h"

#include "cppflask/html/nodes/HtmlNode.h"
#include "cppflask/html/nodes/LoopNode.h"
#include "cppflask/html/StringHelper.h"

using cppflask::html::find;

namespace {
constexpr std::string START_TEXT{"{% FOR "};
constexpr std::string END_TEXT{"{% END_FOR %}"};


std::pair<std::size_t, std::string> findEnd(std::string& _html, std::size_t pos) {
    auto forCount = 1;
    auto nextLoopPos = find(_html, START_TEXT, pos);
    auto endForPos = find(_html, END_TEXT, pos);
    while (forCount > 0) {
        if (nextLoopPos > endForPos) {
            forCount--;
            if (forCount == 0) {
                return {endForPos, _html.substr(endForPos, _html.find("%}", endForPos) - endForPos + 2)};
            }
            endForPos = find(_html, END_TEXT, endForPos + END_TEXT.length());
        } else {
            forCount++;
            nextLoopPos = find(_html, START_TEXT, nextLoopPos + 6);
        }
        if (endForPos == std::string::npos) {
            break;
        }
    }
    return {std::string::npos, ""};
}
}

namespace cppflask::html::parsers::loop {

std::pair<std::size_t, std::unique_ptr<nodes::BaseNode>> LoopParser::parse(std::string& _html, const HtmlCommand& cmd) {
    auto beginStatement = cmd.cmd.find('(') + 1;
    auto endStatement = cmd.cmd.rfind(')');
    auto [endPos, endCommand] = findEnd(_html, cmd.endPos);
    if (beginStatement == std::string::npos || endStatement == std::string::npos) {
        return {endPos + endCommand.length(), std::make_unique<nodes::HtmlNode>("<b>Parse error:</b> Missing brackets.<br />Error occured while parsing: " + cmd.cmd + "<br /><br />")};
    }
    auto expression = cmd.cmd.substr(beginStatement, endStatement - beginStatement);
    auto settings = ExpressionEvaluator::evaluate(expression);
    auto loopContent = _html.substr(cmd.endPos, endPos - cmd.endPos);
    return {endPos + endCommand.length(), std::make_unique<nodes::LoopNode>(settings, loopContent)};
}
}
