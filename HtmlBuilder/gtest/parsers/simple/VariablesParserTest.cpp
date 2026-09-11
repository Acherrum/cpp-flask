#include "gtest/gtest.h"

#include "cppflask/html/parsers/simple/VariablesParser.h"
#include "cppflask/JsonObject.h"
#include "cppflask/html/HtmlCommand.h"

namespace cppflask::html::parsers {

class VariablesParserTest : public testing::Test {};

TEST_F(VariablesParserTest, SimpleVariable) {
    std::string command = "{{$name}}";
    std::string html = command + " Hello World";
    auto cmd = HtmlCommand{html, 0, command.length()};
    auto [endOfData, node] = VariablesParser{html}.parse(cmd);

    {
        JsonObject data{R"raw({"name":"John"})raw"};
        auto result = node->render(data);
        EXPECT_EQ(result, "John");
    }
    {
        JsonObject data{R"raw({"name":"Peter"})raw"};
        auto result = node->render(data);
        EXPECT_EQ(result, "Peter");
    }
}

TEST_F(VariablesParserTest, VariableNotInData) {
    std::string command = "{{$unknown}}";
    std::string html = "Hello " + command;
    auto cmd = HtmlCommand{html, html.find(command), command.length()};
    auto [endOfData, node] = VariablesParser{html}.parse(cmd);

    {
        JsonObject data{R"raw({"user":"Alice"})raw"};
        auto result = node->render(data);
        EXPECT_EQ(result, "");
    }
}
}  // namespace cppflask::html::parsers