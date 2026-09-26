#include "cppflask/html/parsers/simple/VariablesParser.h"

#include "cppflask/html/StringHelper.h"
#include "cppflask/html/nodes/VariableNode.h"
#include "cppflask/html/nodes/PipedVariableNode.h"
#include "cppflask/html/nodes/HtmlNode.h"
#include "cppflask/html/HtmlCommand.h"


namespace cppflask::html::parsers::simple {

std::pair<std::size_t, std::unique_ptr<nodes::BaseNode>> VariablesParser::parse(std::string& html, const HtmlCommand& cmd)
{

    auto fullVariable = html.substr(cmd.startPos + 2, cmd.endPos-cmd.startPos-4);
    stripAll(fullVariable);
    if (fullVariable.at(0) != '$') {
        return {cmd.endPos, std::make_unique<nodes::HtmlNode>(cmd.cmd)};
    }
    auto pipePos = fullVariable.find('|');
    if (pipePos != std::string::npos) {
        return {cmd.endPos, std::make_unique<nodes::PipedVariableNode>(
            fullVariable.substr(1,pipePos-1), fullVariable.substr(pipePos+1))};
    }
    return {cmd.endPos, std::make_unique<nodes::VariableNode>(fullVariable.substr(1))};
}
}
