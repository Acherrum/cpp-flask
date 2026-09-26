#include "gtest/gtest.h"

#include "cppflask/html/FilterRegistry.h"
#include "cppflask/html/parsers/loop/LoopParser.h"
#include "cppflask/html/HtmlCommand.h"
#include "cppflask/JsonObject.h"

namespace cppflask::html::parsers::loop {
class LoopParserTest : public testing::Test {
public:
    JsonObject emptyJson{};
};

TEST_F(LoopParserTest, ClassicForLoop) {
    auto command = std::string{"{% FOR (i = 0 TO 10) %}"};
    auto html = std::string{R"raw({% FOR (i = 0 TO 10) %}{{ $i }} {% END_FOR %})raw"};
    auto cmd = HtmlCommand{
        command, 0, command.length()
    };
    auto [endOfData, node] = LoopParser::parse(html, cmd);
    auto result = node->render(emptyJson);
    ASSERT_EQ("0 1 2 3 4 5 6 7 8 9 ", result);
}

TEST_F(LoopParserTest, ClassicForLoop_Nested) {
    auto html = std::string{R"raw({% FOR (i = 0 TO 2) %}{% FOR (j = 0 TO 2) %}{{ $i }} {{ $j }} {% END_FOR %}{% END_FOR %})raw"};
    auto command = std::string{"{% FOR (i = 0 TO 2) %}"};
    auto cmd = HtmlCommand{
        command, 0, command.length()
    };
    auto [endOfData, node] = LoopParser::parse(html, cmd);
    auto result = node->render(emptyJson);
    ASSERT_EQ("0 0 0 1 1 0 1 1 ", result);
}

TEST_F(LoopParserTest, ClassicForLoop_ImpossibleLoop) {
    auto command = std::string{"{% FOR (i = 3 TO 2) %}"};
    auto html = std::string{R"raw({% FOR (i = 3 TO 2) %}{{ $i }} {% END_FOR %})raw"};
    auto cmd = HtmlCommand{
        command, 0, command.length()
    };
    auto [endOfData, node] = LoopParser::parse(html, cmd);
    auto result = node->render(emptyJson);
    ASSERT_EQ("", result);
}

TEST_F(LoopParserTest, ClassicForLoop_NegativeLoop) {
    auto command = std::string{"{% FOR (i = -5 TO -2) %}"};
    auto html = std::string{R"raw({% FOR (i = -5 TO -2) %}{{ $i }} {% END_FOR %})raw"};
    auto cmd = HtmlCommand{
        command, 0, command.length()
    };
    auto [endOfData, node] = LoopParser::parse(html, cmd);
    auto result = node->render(emptyJson);
    ASSERT_EQ("-5 -4 -3 ", result);
}

TEST_F(LoopParserTest, ClassicForLoop_UsingStartVariable) {
    auto command = std::string{"{% FOR (i = $start TO 6) %}"};
    auto html = std::string{R"raw({% FOR (i = $start TO 6) %}{{ $i }} {% END_FOR %})raw"};
    auto cmd = HtmlCommand{
        command, 0, command.length()
    };
    auto [endOfData, node] = LoopParser::parse(html, cmd);
    JsonObject data{R"raw({"start":3,"end":9})raw"};
    auto result = node->render(data);
    ASSERT_EQ("3 4 5 ", result);
}

TEST_F(LoopParserTest, ClassicForLoop_UsingEndVariable) {
    auto command = std::string{"{% FOR (i = 0 TO $end) %}"};
    auto html = std::string{R"raw({% FOR (i = 0 TO $end) %}{{ $i }} {% END_FOR %})raw"};
    auto cmd = HtmlCommand{
        command, 0, command.length()
    };
    auto [endOfData, node] = LoopParser::parse(html, cmd);
    JsonObject data{R"raw({"start":3,"end":9})raw"};
    auto result = node->render(data);
    ASSERT_EQ("0 1 2 3 4 5 6 7 8 ", result);
}

TEST_F(LoopParserTest, ClassicForLoop_UsingBothVariables) {
    auto command = std::string{"{% FOR (i = $start TO $end) %}"};
    auto html = std::string{R"raw({% FOR (i = $start TO $end) %}{{ $i }} {% END_FOR %})raw"};
    auto cmd = HtmlCommand{
        command, 0, command.length()
    };
    auto [endOfData, node] = LoopParser::parse(html, cmd);
    JsonObject data{R"raw({"start":3,"end":9})raw"};
    auto result = node->render(data);
    ASSERT_EQ("3 4 5 6 7 8 ", result);
}

TEST_F(LoopParserTest, ForEachLoop) {
    auto command = std::string{"{% FOR (el IN $array) %}"};
    auto html = std::string{R"raw({% FOR (el IN $array) %}{{ $el/name }} is {{ $el/job }}.<br />{% END_FOR %})raw"};
    auto cmd = HtmlCommand{
        command, 0, command.length()
    };
    auto [endOfData, node] = LoopParser::parse(html, cmd);
    auto data = JsonObject{R"raw({"array": [{"name":"Peter","job":"spider-man"},{"name":"Bruce","job":"batman"}]})raw"};
    auto result = node->render(data);
    ASSERT_EQ("Peter is spider-man.<br />Bruce is batman.<br />", result);
}

TEST_F(LoopParserTest, ForEachLoop_EmptyList) {
    auto command = std::string{"{% FOR (el IN $array) %}"};
    auto html = std::string{R"raw({% FOR (el IN $array) %}{{ $el/name }} is {{ $el/age }} years old.<br />{% END_FOR %})raw"};
    auto cmd = HtmlCommand{
        command, 0, command.length()
    };
    auto [endOfData, node] = LoopParser::parse(html, cmd);
    auto data = JsonObject{R"raw({"array": []})raw"};
    auto result = node->render(data);
    ASSERT_EQ("", result);
}

TEST_F(LoopParserTest, ForEachLoop_WithFilter) {
    FilterRegistry::registerFilter("print", [](JsonObject& object) {
        return object.getValueAsString("name") + " is " + object.getValueAsString("job");
    });

    auto command = std::string{"{% FOR (el IN $array) %}"};
    auto html = std::string{R"raw({% FOR (el IN $array) %}{{ $el | print }}.<br />{% END_FOR %})raw"};
    auto cmd = HtmlCommand{
        command, 0, command.length()
    };
    auto [endOfData, node] = LoopParser::parse(html, cmd);
    auto data = JsonObject{R"raw({"array": [{"name":"Peter","job":"spider-man"},{"name":"Bruce","job":"batman"}]})raw"};
    auto result = node->render(data);
    ASSERT_EQ("Peter is spider-man.<br />Bruce is batman.<br />", result);
}

TEST_F(LoopParserTest, ClassicForLoop_WithFilter) {
    FilterRegistry::registerFilter("pingPong", [](JsonObject& value) {
        auto result = std::string{};
        auto val = std::stoll(value.getValueAsString(""));
        if (val % 3 == 0) result += "Ping";
        if (val % 5 == 0) result += "Pong";
        if (result.empty()) result = std::to_string(val);
        return result;
    });

    auto command = std::string{"{% FOR (i = 1 TO 10) %}"};
    auto html = std::string{R"raw({% FOR (i = 0 TO 10) %}{{ $i | pingPong }} {% END_FOR %})raw"};
    auto cmd = HtmlCommand{
        command, 0, command.length()
    };
    auto [endOfData, node] = LoopParser::parse(html, cmd);
    auto result = node->render(emptyJson);
    ASSERT_EQ("1 2 Ping 4 Pong Ping 7 8 Ping ", result);
}
}