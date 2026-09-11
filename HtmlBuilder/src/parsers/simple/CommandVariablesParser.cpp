#include "cppflask/html/parsers/simple/CommandVariablesParser.h"

#include <algorithm>

#include "cppflask/JsonObject.h"


namespace cppflask::html::parsers {
CommandVariablesParser::CommandVariablesParser(std::string& html) : _html{html}
{
}

void CommandVariablesParser::parse(const JsonObject &data)
{
    auto startVarPos = _html.find('$');
    while (startVarPos != std::string::npos) {
        auto endVarPos = std::distance(_html.begin(),
                            std::find_if_not(
                                _html.begin() + startVarPos + 1,
                                _html.end(),
                                [](char c) { return std::isalnum(static_cast<unsigned char>(c)) || c == '_' || c == '/'; }
                            ));
        if (endVarPos - startVarPos <= 1) {
            return;
        }
        auto var = _html.substr(startVarPos + 1, endVarPos - startVarPos - 1);
        auto value = data.getValueAsString(var);
        if (!value.empty()) {
            _html = _html.replace(startVarPos, endVarPos-startVarPos, value);
            startVarPos = _html.find('$');
        } else {
            startVarPos = _html.find('$', endVarPos);
        }
    }
}
}
