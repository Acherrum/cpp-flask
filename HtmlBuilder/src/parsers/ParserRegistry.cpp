#include "cppflask/html/parsers/ParserRegistry.h"

#include "cppflask/html/parsers/conditional/ConditionalsParser.h"
#include "cppflask/html/parsers/loop/LoopParser.h"
#include "cppflask/html/parsers/setter/SetParser.h"
#include "cppflask/html/parsers/combine/IncludeParser.h"
#include "cppflask/html/parsers/combine/ExtendsParser.h"
#include "cppflask/html/parsers/combine/BlocksParser.h"

#include "cppflask/html/StringHelper.h"
#include "cppflask/html/nodes/HtmlNode.h"

namespace cppflask::html::parsers {
    ParserRegistry& ParserRegistry::get() {
        static ParserRegistry _instance{};
        return _instance;
    }

    std::pair<std::size_t, std::unique_ptr<nodes::BaseNode>> ParserRegistry::parse(std::string &html, const HtmlCommand &command) {

        for (const auto& [type, parser] : get()._registry) {
            if (contains(command.cmd, std::string{" " + type + " "})) {
                return parser(html, command);
            }
        }
        return {command.endPos, std::make_unique<nodes::HtmlNode>("<b>Parse error:</b> No parser found for: " + command.cmd)};
    }

    ParserRegistry::ParserRegistry() {

        registerParser<conditional::ConditionalsParser>();
        registerParser<loop::LoopParser>();
        registerParser<setter::SetParser>();
        registerParser<IncludeParser>();
        registerParser<ExtendsParser>();
        registerParser<BlocksParser>();
    }
}
