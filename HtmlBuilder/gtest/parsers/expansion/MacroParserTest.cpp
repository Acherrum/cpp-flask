#include "gtest/gtest.h"

#include "cppflask/html/parsers/expansion/MacroParser.h"
#include "cppflask/html/HtmlBuilder.h"
#include "cppflask/html/HtmlCommand.h"
#include "cppflask/JsonObject.h"
#include "cppflask/html/parsers/MacroRegistry.h"


namespace cppflask::html::parsers::expansion {
class MacroParserTest : public testing::Test {
public:
    std::string _importPrint{"{% macro print from (macroTest.file) %}"};
    std::string _importBanner{"{% macro banner from (macroTest.file) %}"};
    std::string _macroCmd{"{% macro print(message) %}"};
    std::string _fullMacro{_macroCmd + "{{ $message }}{% end_macro %}"};
};

TEST_F(MacroParserTest, MacroDefinition_LoadsIntoMemory_NoDirectHtmlImpact) {
    auto html = _fullMacro;
    HtmlCommand command{_macroCmd, 0, _macroCmd.length()};

    auto [position, node] = MacroParser::parse(html, command);
    ASSERT_EQ(position, 0);
    ASSERT_EQ(nullptr, node);
    ASSERT_TRUE(html.empty());
    ASSERT_NE(std::nullopt, MacroRegistry::get("print"));
    ASSERT_EQ(1UL, MacroRegistry::get("print")->arguments.size());
    ASSERT_EQ("message", MacroRegistry::get("print")->arguments.at(0));
    ASSERT_EQ("{{ $message }}", MacroRegistry::get("print")->body);
}

TEST_F(MacroParserTest, ImportLoadsMacroIntoHtmlForSecondParsing_print) {

    auto html = _importPrint;
    HtmlCommand command{_importPrint, 0, _importPrint.length()};

    auto [position, node] = MacroParser::parse(html, command);
    ASSERT_EQ(position, 0);
    ASSERT_EQ(nullptr, node);
    ASSERT_TRUE(html.empty());
    ASSERT_NE(std::nullopt, MacroRegistry::get("print"));
    ASSERT_EQ(1UL, MacroRegistry::get("print")->arguments.size());
    ASSERT_EQ("message", MacroRegistry::get("print")->arguments.at(0));
    ASSERT_EQ("{{ $message }}", MacroRegistry::get("print")->body);
}

TEST_F(MacroParserTest, ImportLoadsMacroIntoHtmlForSecondParsing_banner) {

    auto html = _importBanner;
    HtmlCommand command{_importBanner, 0, _importBanner.length()};

    auto [position, node] = MacroParser::parse(html, command);
    ASSERT_EQ(position, 0);
    ASSERT_EQ(nullptr, node);
    ASSERT_TRUE(html.empty());
    ASSERT_NE(std::nullopt, MacroRegistry::get("banner"));
    ASSERT_EQ(3UL, MacroRegistry::get("banner")->arguments.size());
    ASSERT_EQ("rows", MacroRegistry::get("banner")->arguments.at(0));
    ASSERT_EQ("columns", MacroRegistry::get("banner")->arguments.at(1));
    ASSERT_EQ("message", MacroRegistry::get("banner")->arguments.at(2));
}
} // namespace cppflask::html::parsers::expansion
