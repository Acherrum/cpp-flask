#pragma once

#include <unordered_map>
#include <string>
#include <memory>
#include <optional>

#include "cppflask/html/HtmlCommand.h"
#include "cppflask/html/nodes/BaseNode.h"
#include "cppflask/html/parsers/MacroDefinition.h"

namespace cppflask::html {
struct HtmlCommand;

namespace parsers {
    class ParserRegistry {
    public:

        static std::pair<std::size_t, std::unique_ptr<nodes::BaseNode>> parse(std::string& html, const HtmlCommand& command);
        static void finish();

        ParserRegistry(const ParserRegistry&) = delete;
        ParserRegistry& operator=(const ParserRegistry&) = delete;
        ParserRegistry(ParserRegistry&&) = delete;
        ParserRegistry& operator=(ParserRegistry&&) = delete;

    private:
        using ParseFunction = std::pair<std::size_t, std::unique_ptr<nodes::BaseNode>>(*)(std::string& html, const HtmlCommand& command);
        std::unordered_map<std::string, ParseFunction> _registry{};

        static ParserRegistry& get();
        ParserRegistry();

        template <typename PARSER>
        bool registerParser() {
            _registry[PARSER::TYPE] = PARSER::parse;
            return true;
        }
    };
}
}