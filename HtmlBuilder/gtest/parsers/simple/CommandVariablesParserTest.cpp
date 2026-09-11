#include "gtest/gtest.h"

#include "cppflask/html/parsers/simple/CommandVariablesParser.h"
#include "cppflask/JsonObject.h"

namespace cppflask::html::parsers {

class CommandVariablesParserTest : public testing::Test {};

TEST_F(CommandVariablesParserTest, SimpleVariable) {
    std::string html = "$name Hello World";
    JsonObject data{R"raw({"name":"John"})raw"};
    auto parser = CommandVariablesParser{html};
    parser.parse(data);
    EXPECT_EQ(html, "John Hello World");
}

TEST_F(CommandVariablesParserTest, VariableAtEndOfString) {
    std::string html = "Hello $user";
    JsonObject data{R"raw({"user":"Alice"})raw"};
    auto parser = CommandVariablesParser{html};
    parser.parse(data);
    EXPECT_EQ(html, "Hello Alice");
}

TEST_F(CommandVariablesParserTest, VariableWithDelimiter) {
    std::string html = "$name : $age";
    JsonObject data{R"raw({"name":"Bob","age":"25"})raw"};
    auto parser = CommandVariablesParser{html};
    parser.parse(data);
    EXPECT_EQ(html, "Bob : 25");
}

TEST_F(CommandVariablesParserTest, MissingVariable) {
    std::string html = "$unknown";
    JsonObject data{R"raw({"name":"John"})raw"};
    auto parser = CommandVariablesParser{html};
    parser.parse(data);
    EXPECT_EQ(html, "$unknown");
}

TEST_F(CommandVariablesParserTest, MultipleVariablesSameLine) {
    std::string html = "$first + $second";
    JsonObject data{R"raw({"first":"10","second":"20"})raw"};
    auto parser = CommandVariablesParser{html};
    parser.parse(data);
    EXPECT_EQ(html, "10 + 20");
}

TEST_F(CommandVariablesParserTest, VariableAfterReplacement) {
    std::string html = "$one$two";
    JsonObject data{R"raw({"one":"A","two":"B"})raw"};
    auto parser = CommandVariablesParser{html};
    parser.parse(data);
    EXPECT_EQ(html, "AB");
}

TEST_F(CommandVariablesParserTest, EmptyString) {
    std::string html = "";
    JsonObject data{};
    auto parser = CommandVariablesParser{html};
    parser.parse(data);
    EXPECT_EQ(html, "");
}

TEST_F(CommandVariablesParserTest, NoDollarSign) {
    std::string html = "Hello world";
    JsonObject data{R"raw({"name":"John"})raw"};
    auto parser = CommandVariablesParser{html};
    parser.parse(data);
    EXPECT_EQ(html, "Hello world");
}

TEST_F(CommandVariablesParserTest, UnderscoreInVariableName) {
    std::string html = "$first_name";
    JsonObject data{R"raw({"first_name":"Jane"})raw"};
    auto parser = CommandVariablesParser{html};
    parser.parse(data);
    EXPECT_EQ(html, "Jane");
}

TEST_F(CommandVariablesParserTest, EmptyVariableName) {
    std::string html = "$";
    JsonObject data{};
    auto parser = CommandVariablesParser{html};
    parser.parse(data);
    EXPECT_EQ(html, "$");
}

TEST_F(CommandVariablesParserTest, MixedVariablesAndStaticText) {
    std::string html = "Hello $name! Welcome to $site.";
    JsonObject data{R"raw({"name":"World","site":"cppflask"})raw"};
    auto parser = CommandVariablesParser{html};
    parser.parse(data);
    EXPECT_EQ(html, "Hello World! Welcome to cppflask.");
}

TEST_F(CommandVariablesParserTest, VariableAtStartOfString) {
    std::string html = "$x + $y";
    JsonObject data{R"raw({"x":"1","y":"2"})raw"};
    auto parser = CommandVariablesParser{html};
    parser.parse(data);
    EXPECT_EQ(html, "1 + 2");
}

TEST_F(CommandVariablesParserTest, VariableWithSpecialCharactersInDelimiter) {
    std::string html = "$name[0] = $value";
    JsonObject data{R"raw({"name":"myarray","value":"42"})raw"};
    auto parser = CommandVariablesParser{html};
    parser.parse(data);
    EXPECT_EQ(html, "myarray[0] = 42");
}

TEST_F(CommandVariablesParserTest, VariableEndingAtNewline) {
    std::string html = "$text\n$more";
    JsonObject data{R"raw({"text":"First","more":"Second"})raw"};
    auto parser = CommandVariablesParser{html};
    parser.parse(data);
    EXPECT_EQ(html, "First\nSecond");
}

TEST_F(CommandVariablesParserTest, DollarSignNotAtStartIsNotReplaced) {
    std::string html = "$notAVar $name";
    JsonObject data{R"raw({"name":"John"})raw"};
    auto parser = CommandVariablesParser{html};
    parser.parse(data);
    EXPECT_EQ(html, "$notAVar John");
}

TEST_F(CommandVariablesParserTest, VariableFollowedByOnlyDigitsIsReplaced) {
    std::string html = "$count5";
    JsonObject data{R"raw({"count5":"five"})raw"};
    auto parser = CommandVariablesParser{html};
    parser.parse(data);
    EXPECT_EQ(html, "five");
}

TEST_F(CommandVariablesParserTest, VariableFollowedByDigitIsReplaced) {
    std::string html = "$var 42";
    JsonObject data{R"raw({"var":"replaced"})raw"};
    auto parser = CommandVariablesParser{html};
    parser.parse(data);
    EXPECT_EQ(html, "replaced 42");
}

TEST_F(CommandVariablesParserTest, VariableFollowedByPlusIsReplaced) {
    std::string html = "$name + $age";
    JsonObject data{R"raw({"name":"Bob","age":"30"})raw"};
    auto parser = CommandVariablesParser{html};
    parser.parse(data);
    EXPECT_EQ(html, "Bob + 30");
}

TEST_F(CommandVariablesParserTest, VariableWithHyphenIfSupportedByJsonObject) {
    // Hyphen is non-alphanumeric so it terminates the variable name immediately
    std::string html = "$a-b";
    JsonObject data{R"raw({"a":"b"})raw"};
    auto parser = CommandVariablesParser{html};
    parser.parse(data);
    EXPECT_EQ(html, "b-b");
}

TEST_F(CommandVariablesParserTest, VariableContainingDot) {
    std::string html = "$my.object";
    JsonObject data{R"raw({"my":"some"})raw"};
    auto parser = CommandVariablesParser{html};
    parser.parse(data);
    EXPECT_EQ(html, "some.object");
}

TEST_F(CommandVariablesParserTest, MultipleSeparateReplacements) {
    std::string html = "$a $b $c";
    JsonObject data{R"raw({"a":"A","b":"B","c":"C"})raw"};
    auto parser = CommandVariablesParser{html};
    parser.parse(data);
    EXPECT_EQ(html, "A B C");
}

TEST_F(CommandVariablesParserTest, ReplacementsPreserveDelimiterCharacters) {
    std::string html = "$name $age years old";
    JsonObject data{R"raw({"name":"Alice","age":17})raw"};
    auto parser = CommandVariablesParser{html};
    parser.parse(data);
    EXPECT_EQ(html, "Alice 17 years old");
}

}  // namespace cppflask::html::parsers