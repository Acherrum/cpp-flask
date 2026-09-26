#include "cppflask/html/parsers/simple/MacroParser.h"

#include "cppflask/html/nodes/HtmlNode.h"
#include "cppflask/html/nodes/MacroNode.h"
#include "cppflask/html/HtmlCommand.h"
#include "cppflask/html/parsers/MacroRegistry.h"
#include "cppflask/html/StringHelper.h"

namespace {

std::vector<std::string> parseArguments(const std::string& argumentsDefinition) {
    std::vector<std::string> arguments;
    auto currentArg = std::string{};
    bool insideQuotes = false;

    for (char ch : argumentsDefinition) {
        if (ch == '"') {
            insideQuotes = !insideQuotes;
        } else if (ch == ',' && !insideQuotes) {
            cppflask::html::trim(currentArg);
            arguments.push_back(currentArg);
            currentArg.clear();
        } else {
            currentArg += ch;
        }
    }

    if (!currentArg.empty()) {
        cppflask::html::trim(currentArg);
        arguments.push_back(currentArg);
    }

    return arguments;
}

std::vector<std::string> getArguments(const std::string& argumentsDefinition) {
    
    auto argumentListStartPos = argumentsDefinition.find('(') + 1;
    auto argumentListEndPos = argumentsDefinition.rfind(')');
    auto arguments = cppflask::html::trim(argumentsDefinition.substr(argumentListStartPos, argumentListEndPos - argumentListStartPos));
    return parseArguments(arguments);
}
}

namespace cppflask::html::parsers::simple {
std::pair<std::size_t, std::unique_ptr<nodes::BaseNode>> MacroParser::parse(std::string&, const HtmlCommand& cmd) {
        
    auto macroNameStartPos = cmd.cmd.find_first_not_of(' ', 2);
    auto macroNameEndPos = cmd.cmd.find_first_of(" (", macroNameStartPos + 1);
    auto macroName = cmd.cmd.substr(macroNameStartPos, macroNameEndPos - macroNameStartPos);
    auto macro = MacroRegistry::get(macroName);
    if (!macro.has_value()) {
        return {cmd.endPos, std::make_unique<nodes::HtmlNode>("Parse error: macro does not exist: " + cmd.cmd)};
    }

    auto arguments = getArguments(cmd.cmd);
    if (macro->arguments.size() != arguments.size()) {
        return {cmd.endPos, std::make_unique<nodes::HtmlNode>("Parse error: argument count does not match: " + cmd.cmd)};
    }

    return {cmd.endPos, std::make_unique<nodes::MacroNode>(macro->body, macro->arguments, arguments)};
}
}