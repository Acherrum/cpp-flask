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
            return std::to_string(array.getSize());
        });

        auto html = std::string{"{{ $test | getArrayLength }}"};
        auto command = HtmlCommand{html,0,html.length()};
        auto [dontCare, node] = VariablesParser::parse(html, command);

        auto json = JsonObject(R"raw({"test":[1,3,3,7]})raw");
        auto result = node->render(json);
        ASSERT_EQ("4", result);
    }

    TEST_F(PipedVariableNodeTest, MultiPipeMagic) {

        FilterRegistry::registerFilter("sortById", [](JsonObject& array) {
            array.sort(SortOrder::Ascending, "id");
            return array.toString();
        });
        FilterRegistry::registerFilter("getIdsBelow10", [](JsonObject& array) {
            auto newArray = JsonObject{};
            for (int i = 0, index = 0; i < array.getSize(); i++) {
                if (array.getValue(std::to_string(i) + "/id", 0L) < 10) {
                    newArray.set("/" + std::to_string(index), array.get(std::to_string(i)));
                    index++;
                } else {
                    break;
                }
            }
            return newArray.toString();
        });

        auto html = std::string{"{{ $test | sortById | getIdsBelow10 }}"};
        auto command = HtmlCommand{html,0,html.length()};
        auto [dontCare, node] = VariablesParser::parse(html, command);

        auto json = JsonObject(R"raw({"test":[{"id":10,"name":"not this one"},{"id":2,"name":"this one too"},{"id":1,"name":"this one"},{"id":12,"name":"nor this one"}]})raw");
        auto result = node->render(json);
        ASSERT_EQ(R"([{"id":1,"name":"this one"},{"id":2,"name":"this one too"}])", result);
    }
}
