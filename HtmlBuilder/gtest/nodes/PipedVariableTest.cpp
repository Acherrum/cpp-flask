#include "gtest/gtest.h"

#include "cppflask/html/parsers/simple/VariablesParser.h"
#include "cppflask/JsonObject.h"
#include "cppflask/html/FilterRegistry.h"
#include "cppflask/html/HtmlCommand.h"

using cppflask::html::parsers::simple::VariablesParser;

namespace cppflask::html::nodes {
    class PipedVariableNodeTest : public testing::Test {
    };

    TEST_F(PipedVariableNodeTest, NonExistantFilterSimplySerializesData) {

        auto html = std::string{"{{ $test | doesNotExist }}"};
        auto command = HtmlCommand{html,0,html.length()};
        auto [dontCare, node] = VariablesParser::parse(html, command);

        auto json = JsonObject(R"raw({"test":[1,2,3,4]})raw");
        auto result = node->render(json);
        ASSERT_EQ("[1,2,3,4]", result);
    }

    TEST_F(PipedVariableNodeTest, RegisteredFilterIsProperlyCalled) {

        FilterRegistry::registerFilter("getArrayLength", [](JsonObject& array) {
            return std::to_string(array.getArraySize());
        });

        auto html = std::string{"{{ $test | getArrayLength }}"};
        auto command = HtmlCommand{html,0,html.length()};
        auto [dontCare, node] = VariablesParser::parse(html, command);

        auto json = JsonObject(R"raw({"test":[1,3,3,7]})raw");
        auto result = node->render(json);
        ASSERT_EQ("4", result);
    }
}
