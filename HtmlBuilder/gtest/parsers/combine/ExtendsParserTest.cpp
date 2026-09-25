
#include "gtest/gtest.h"

#include "cppflask/html/parsers/combine/ExtendsParser.h"
#include "cppflask/JsonObject.h"
#include "cppflask/html/HtmlCommand.h"
#include "cppflask/html/StringHelper.h"
#include "cppflask/html/HtmlBuilder.h"

namespace cppflask::html::parsers {

class ExtendsParserTest : public testing::Test {

};

TEST_F(ExtendsParserTest, multipleExtensions) {
    auto itBeginsHere = std::string{R"raw({% extends (extendsBase.file) %}

        {% block nestedInBase %}
        Hello, world!
        {% end_block %}
    )raw"};
    auto result = HtmlBuilder::fromText(itBeginsHere).build();
    stripAll(result);
    ASSERT_EQ("[[\n-->\nHello,world!\n<--\n]]", result);
}

TEST_F(ExtendsParserTest, optionalBlockShouldNotThrowParseError_WhenUsed) {
    auto itBeginsHere = std::string{R"raw({% extends (blockForAdditionalExtension.file) %}

        {% block optionalExtension %}Optional block is used{% end_block %}
    )raw"};
    auto result = HtmlBuilder::fromText(itBeginsHere).build();
    stripAll(result);
    ASSERT_EQ("[[Thisgoesintobase.\n\n[optional]Optionalblockisused[/optional]]]", result);
}

TEST_F(ExtendsParserTest, optionalBlockShouldNotThrowParseError_WhenNotUsed) {
    
    auto result = HtmlBuilder::fromFile("blockForAdditionalExtension.file").build();
    stripAll(result);
    ASSERT_EQ("[[Thisgoesintobase.[optional][/optional]]]", result);
}

TEST_F(ExtendsParserTest, blocksCanBePassedDownTheLine) {
    auto itBeginsHere = std::string{R"raw({% extends (multipleBlocksIntermediate.file) %}

        {% block two %}This is two{% end_block %}
    )raw"};
    
    auto result = HtmlBuilder::fromText(itBeginsHere).build();
    stripAll(result);
    ASSERT_EQ("1)thisisone\n2)Thisistwo", result);
}
}