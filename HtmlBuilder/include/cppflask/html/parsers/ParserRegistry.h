#pragma once

#include <unordered_map>
#include <string>
#include <memory>

#include "cppflask/html/HtmlCommand.h"
#include "cppflask/html/nodes/BaseNode.h"


namespace cppflask::html {
struct HtmlCommand;

namespace parsers {
    class ParserRegistry {
    public:
        static ParserRegistry get();

        std::pair<std::size_t, std::unique_ptr<nodes::BaseNode>> parse(std::string& html, const HtmlCommand& command);

    private:
        using ParseFunction = std::pair<std::size_t, std::unique_ptr<nodes::BaseNode>>(*)(std::string& html, const HtmlCommand& command);
        std::unordered_map<std::string, ParseFunction> _registry{};

        ParserRegistry();

        template <typename PARSER>
        bool registerParser() {
            _registry[PARSER::TYPE] = PARSER::parse;
            return true;
        }
    };
}
}