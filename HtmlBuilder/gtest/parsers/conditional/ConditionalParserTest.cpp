#include "gtest/gtest.h"

#include "cppflask/html/parsers/conditional/ConditionalsParser.h"
#include "cppflask/html/HtmlCommand.h"
#include "cppflask/JsonObject.h"

namespace cppflask::html::parsers::conditional {
class ConditionalsParserTest : public testing::Test {
public:
    std::string command{"{% IF ($hello) %}"};
    std::string html{R"raw({% IF ($hello) %}hello{% END_IF %})raw"};
    HtmlCommand cmd{
        command, 0, command.length()
    };
    ConditionalsParser parser{html};
};

TEST_F(ConditionalsParserTest, SimpleIfStatement_TRUE) {

    auto [endOfData, node] = parser.parse(cmd);
    ASSERT_EQ(endOfData, html.length());
    auto data = JsonObject{"{\"hello\":true}"};
    auto result = node->render(data);
    ASSERT_EQ("hello", result);
}

TEST_F(ConditionalsParserTest, SimpleIfStatement_FALSE) {

    auto [endOfData, node] = parser.parse(cmd);
    ASSERT_EQ(endOfData, html.length());
    auto data = JsonObject{"{\"hello\":false}"};
    auto result = node->render(data);
    ASSERT_TRUE(result.empty());
}
}