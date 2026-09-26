#include "cppflask/html/parsers/simple/SimpleParser.h"

#include "cppflask/html/HtmlCommand.h"
#include "cppflask/html/parsers/simple/MacroParser.h"
#include "cppflask/html/parsers/simple/VariablesParser.h"

namespace cppflask::html::parsers::simple {


std::pair<std::size_t, std::unique_ptr<nodes::BaseNode>> SimpleParser::parse(std::string& html, const HtmlCommand& cmd) {
        
    if (cmd.cmd.find('(') != std::string::npos) {
        return MacroParser::parse(html, cmd);
    } else {
        return VariablesParser::parse(html, cmd);
    }
    
}
}