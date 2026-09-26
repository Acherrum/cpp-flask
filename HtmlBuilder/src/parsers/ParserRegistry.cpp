#include "cppflask/html/parsers/ParserRegistry.h"

#include "cppflask/html/parsers/conditional/ConditionalsParser.h"
#include "cppflask/html/parsers/loop/LoopParser.h"
#include "cppflask/html/parsers/setter/SetParser.h"
#include "cppflask/html/parsers/expansion/IncludeParser.h"
#include "cppflask/html/parsers/expansion/ExtendsParser.h"
#include "cppflask/html/parsers/expansion/BlocksParser.h"
#include "cppflask/html/parsers/expansion/MacroParser.h"
#include "cppflask/html/parsers/simple/SimpleParser.h"

#include "cppflask/html/parsers/MacroRegistry.h"
#include "cppflask/html/StringHelper.h"
#include "cppflask/html/nodes/HtmlNode.h"

namespace cppflask::html::parsers {
    ParserRegistry& ParserRegistry::get() {
        static ParserRegistry _instance{};
        return _instance;
    }

    std::pair<std::size_t, std::unique_ptr<nodes::BaseNode>> ParserRegistry::parse(std::string &html, const HtmlCommand &command) {

        if (command.cmd.find("{{") == 0) {
            return simple::SimpleParser::parse(html, command);
        } else {
            for (const auto& [type, parser] : get()._registry) {
                if (contains(command.cmd, std::string{" " + type + " "})) {
                    return parser(html, command);
                }
            }
            return {command.endPos, std::make_unique<nodes::HtmlNode>("<b>Parse error:</b> No parser found for: " + command.cmd)};
        }
    }

    void ParserRegistry::finish() {
        
        MacroRegistry::clear();
    }

    ParserRegistry::ParserRegistry() {

        registerParser<conditional::ConditionalsParser>();
        registerParser<loop::LoopParser>();
        registerParser<setter::SetParser>();
        registerParser<expansion::IncludeParser>();
        registerParser<expansion::ExtendsParser>();
        registerParser<expansion::BlocksParser>();
        registerParser<expansion::MacroParser>();
    }
}
