#include "gtest/gtest.h"

#include "cppflask/html/parsers/conditional/ConditionalsParser.h"
#include "cppflask/html/HtmlCommand.h"
#include "cppflask/JsonObject.h"

namespace cppflask::html::parsers::conditional {
class ConditionalsParserTest : public testing::Test {
public:
    std::string _command{"{% IF ($hello) %}"};
    std::string _html{R"raw({% IF ($hello) %}hello{% END_IF %})raw"};
    HtmlCommand _cmd{
        _command, 0, _command.length()
    };
};

TEST_F(ConditionalsParserTest, SimpleIfStatement_TRUE) {

    auto [endOfData, node] = ConditionalsParser::parse(_html, _cmd);
    ASSERT_EQ(endOfData, _html.length());
    auto data = JsonObject{"{\"hello\":true}"};
    auto result = node->render(data);
    ASSERT_EQ("hello", result);
}

TEST_F(ConditionalsParserTest, SimpleIfStatement_FALSE) {

    auto [endOfData, node] = ConditionalsParser::parse(_html, _cmd);
    ASSERT_EQ(endOfData, _html.length());
    auto data = JsonObject{"{\"hello\":false}"};
    auto result = node->render(data);
    ASSERT_TRUE(result.empty());
}

TEST_F(ConditionalsParserTest, IfElseIfWithAndLogic_AAndB) {
    auto command = std::string{"{% IF ($a AND $b) %}"};
    auto html = std::string{"{% IF ($a AND $b) %}a and b{% ELSE IF ($a) %}only a{% ELSE IF ($b) %}only b{% ELSE %}neither{% END_IF %}"};
    HtmlCommand cmd{
        command, 0, command.length()
    };
    auto [endOfData, node] = ConditionalsParser::parse(html, cmd);

    auto data = JsonObject{R"raw({"a":true, "b": true})raw"};
    auto result = node->render(data);
    ASSERT_EQ("a and b", result);
}

TEST_F(ConditionalsParserTest, IfElseIfWithAndLogic_OnlyA) {
    auto command = std::string{"{% IF ($a AND $b) %}"};
    auto html = std::string{"{% IF ($a AND $b) %}a and b{% ELSE IF ($a) %}only a{% ELSE IF ($b) %}only b{% ELSE %}neither{% END_IF %}"};
    HtmlCommand cmd{
        command, 0, command.length()
    };
    auto [endOfData, node] = ConditionalsParser::parse(html, cmd);

    auto data = JsonObject{R"raw({"a":true, "b": false})raw"};
    auto result = node->render(data);
    ASSERT_EQ("only a", result);
}

TEST_F(ConditionalsParserTest, IfElseIfWithAndLogic_OnlyB) {
    auto command = std::string{"{% IF ($a AND $b) %}"};
    auto html = std::string{"{% IF ($a AND $b) %}a and b{% ELSE IF ($a) %}only a{% ELSE IF ($b) %}only b{% ELSE %}neither{% END_IF %}"};
    HtmlCommand cmd{
        command, 0, command.length()
    };
    auto [endOfData, node] = ConditionalsParser::parse(html, cmd);

    auto data = JsonObject{R"raw({"a":false, "b": true})raw"};
    auto result = node->render(data);
    ASSERT_EQ("only b", result);
}

TEST_F(ConditionalsParserTest, IfElseIfWithAndLogic_Neither) {
    auto command = std::string{"{% IF ($a AND $b) %}"};
    auto html = std::string{"{% IF ($a AND $b) %}a and b{% ELSE IF ($a) %}only a{% ELSE IF ($b) %}only b{% ELSE %}neither{% END_IF %}"};
    HtmlCommand cmd{
        command, 0, command.length()
    };
    auto [endOfData, node] = ConditionalsParser::parse(html, cmd);

    auto data = JsonObject{R"raw({"a":false, "b": false})raw"};
    auto result = node->render(data);
    ASSERT_EQ("neither", result);
}

TEST_F(ConditionalsParserTest, IfElseWithAndLogic_TruePath) {
    auto command = std::string{"{% IF ($text != \"\" AND $first == $second) %}"};
    auto html = std::string{"{% IF ($text != \"\" AND $first == $second) %}true{% ELSE %}false{% END_IF %}"};
    HtmlCommand cmd{
        command, 0, command.length()
    };
    auto [endOfData, node] = ConditionalsParser::parse(html, cmd);

    auto data = JsonObject{R"raw({"text":"not empty", "first": "banana", "second":"banana"})raw"};
    auto result = node->render(data);
    ASSERT_EQ("true", result);
}

TEST_F(ConditionalsParserTest, IfElseWithAndLogic_FalsePath_ShortCircuit) {
    auto command = std::string{"{% IF ($text != \"\" AND $first == $second) %}"};
    auto html = std::string{"{% IF ($text != \"\" AND $first == $second) %}true{% ELSE %}false{% END_IF %}"};
    HtmlCommand cmd{
        command, 0, command.length()
    };
    auto [endOfData, node] = ConditionalsParser::parse(html, cmd);

    auto data = JsonObject{R"raw({"text":"", "first": "banana", "second":"banana"})raw"};
    auto result = node->render(data);
    ASSERT_EQ("false", result);
}


TEST_F(ConditionalsParserTest, IfElseWithAndLogic_FalsePath) {
    auto command = std::string{"{% IF ($text != \"\" AND $first == $second) %}"};
    auto html = std::string{"{% IF ($text != \"\" AND $first == $second) %}true{% ELSE %}false{% END_IF %}"};
    HtmlCommand cmd{
        command, 0, command.length()
    };
    auto [endOfData, node] = ConditionalsParser::parse(html, cmd);

    auto data = JsonObject{R"raw({"text":"not empty", "first": "banana", "second":"bananas"})raw"};
    auto result = node->render(data);
    ASSERT_EQ("false", result);
}
}