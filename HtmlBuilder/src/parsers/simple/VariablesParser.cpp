#include "cppflask/html/parsers/simple/VariablesParser.h"

#include "cppflask/html/StringHelper.h"
#include "cppflask/html/nodes/VariableNode.h"
#include "cppflask/html/nodes/HtmlNode.h"
#include "cppflask/html/HtmlCommand.h"


namespace cppflask::html::parsers {
VariablesParser::VariablesParser(std::string& html) : _html{html}
{
}

std::pair<std::size_t, std::unique_ptr<nodes::BaseNode>> VariablesParser::parse(const HtmlCommand& cmd)
{

    auto fullVariable = _html.substr(cmd.startPos + 2, cmd.endPos-cmd.startPos-4);
    stripAll(fullVariable);
    if (fullVariable.at(0) != '$') {
        return {cmd.endPos, std::make_unique<nodes::HtmlNode>(cmd.cmd)};
    }
    return {cmd.endPos, std::make_unique<nodes::VariableNode>(fullVariable.substr(1))};
}
}
