#include "gtest/gtest.h"

#include "cppflask/html/parsers/simple/MacroParser.h"
#include "cppflask/JsonObject.h"
#include "cppflask/html/HtmlBuilder.h"
#include "cppflask/html/StringHelper.h"


namespace cppflask::html::parsers::simple {
class SimpleMacroParserTest : public testing::Test {};

TEST_F(SimpleMacroParserTest, MultipleMacroCallsWithoutData) {

    auto result = HtmlBuilder::fromText(R"raw(
    {% macro print from (macroTest.file) %}

    {{ print("Hello, world!") }} {{ print("This is a test.") }} {{ print(Yes!) }}
    )raw").build();
    trim(result);
    ASSERT_EQ("Hello, world! This is a test. Yes!", result);
}

TEST_F(SimpleMacroParserTest, MultipleMacroCallsWithData) {

    auto data = JsonObject{R"({"one": 1, "two": 2, "three": 3})"};
    auto result = HtmlBuilder::fromText(R"raw(
    {% macro print from (macroTest.file) %}

    {{ print($one) }} {{ print($two) }} {{ print($three) }}
    )raw").buildWithData(data);
    trim(result);
    ASSERT_EQ("1 2 3", result);
}

TEST_F(SimpleMacroParserTest, DISABLED_MultipleArgumentsWithData) {

    auto data = JsonObject{R"({"one": 2, "two": 2, "three": "hello"})"};
    auto result = HtmlBuilder::fromText(R"raw(
    {% macro banner from (macroTest.file) %}

    {{ banner($one, $two, $three) }}
    )raw").buildWithData(data);
    trim(result);
    ASSERT_EQ("Hello => [0, 0]Hello => [0, 1]Hello => [1, 0]Hello => [1, 1]", result);
}


}