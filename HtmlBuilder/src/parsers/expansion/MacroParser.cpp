#include "cppflask/html/parsers/expansion/MacroParser.h"

#include <fstream>
#include <sstream>
#include <vector>
#include <ranges>

#include "cppflask/html/nodes/HtmlNode.h"
#include "cppflask/html/nodes/MacroNode.h"
#include "cppflask/html/HtmlCommand.h"
#include "cppflask/html/StringHelper.h"
#include "cppflask/html/parsers/MacroRegistry.h"

using cppflask::html::contains;
using cppflask::html::find;
using cppflask::html::stripAll;

namespace {
const std::string END_MACRO{"{% end_macro %}"};

std::string getMacroFromFile(const std::string& filename, const std::string& macroName) {
    
    std::ifstream file(filename);
    if (!file.is_open()) {
        return "Parse error: could not open file " + filename;
    }

    auto macro = std::string{};
    auto line = std::string{};
    bool copyMacro = false;
    while (std::getline(file, line)) {
        if (copyMacro || contains(line, macroName)) {
            macro += line;
            copyMacro = find(line, END_MACRO) == std::string::npos;
            if (!copyMacro) {
                break;
            }
        }
    }

    file.close();
    return macro;
}

std::vector<std::string> getArguments(const std::string& argumentsDefinition) {
    
    auto argumentListStartPos = argumentsDefinition.find('(') + 1;
    auto argumentListEndPos = argumentsDefinition.rfind(')');
    auto arguments = stripAll(argumentsDefinition.substr(argumentListStartPos, argumentListEndPos - argumentListStartPos));
    auto result = arguments | std::views::split(',') | std::ranges::to<std::vector<std::string>>();
    return result;
}

cppflask::html::parsers::MacroDefinition parseFullMacro(const std::string& fullMacro) {

    auto macroDefPos = find(fullMacro, " macro ");
    auto macroNameStartPos = fullMacro.find_first_not_of(' ', macroDefPos + 7);
    auto macroNameEndPos = fullMacro.find_first_of(" (", macroNameStartPos + 1);
    auto macroName = fullMacro.substr(macroNameStartPos, macroNameEndPos - macroNameStartPos);
    auto macroDefCommandEnd = fullMacro.find("%}", macroNameEndPos);
    auto arguments = getArguments(fullMacro.substr(0, macroDefCommandEnd));
    auto bodyStart = macroDefCommandEnd + 2;
    auto bodyEnd = find(fullMacro, END_MACRO);
    auto body = fullMacro.substr(bodyStart, bodyEnd - bodyStart);
    return {macroName, std::move(arguments), body };
}
}

namespace cppflask::html::parsers::expansion {
std::pair<std::size_t, std::unique_ptr<nodes::BaseNode>> MacroParser::parse(std::string& html, const HtmlCommand& cmd) {

    auto macroDefPos = find(cmd.cmd, " macro ");

    auto macroHtml = std::string{};
    auto importPos = find(cmd.cmd, " from ");
    if (importPos != std::string::npos) {
        auto macroNameStartPos = cmd.cmd.find_first_not_of(' ', macroDefPos + 7);
        auto macroNameEndPos = cmd.cmd.find_first_of(" (", macroNameStartPos + 1);
        auto macroName = cmd.cmd.substr(macroNameStartPos, macroNameEndPos - macroNameStartPos);
        auto filenameStartPos = cmd.cmd.find('(', importPos) + 1;
        auto filename = cmd.cmd.substr(filenameStartPos, cmd.cmd.rfind(')') - filenameStartPos);
        macroHtml = getMacroFromFile(filename, macroName);
        html.erase(cmd.startPos, cmd.cmd.length());
    } else {
        auto macroEndPos = html.find(END_MACRO, cmd.endPos);
        macroHtml = html.substr(cmd.startPos, macroEndPos + END_MACRO.length() - cmd.startPos);
        html.erase(cmd.startPos, macroEndPos + END_MACRO.length() - cmd.startPos);
    }

    MacroRegistry::save(parseFullMacro(macroHtml));

    return {cmd.startPos, nullptr};
}

}