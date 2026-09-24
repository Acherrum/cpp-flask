
#include "gtest/gtest.h"

#include "cppflask/html/parsers/combine/ExtendsParser.h"
#include "cppflask/JsonObject.h"
#include "cppflask/html/HtmlCommand.h"
#include "cppflask/html/StringHelper.h"

namespace cppflask::html::parsers {

class ExtendsParserTest : public testing::Test {};

TEST_F(ExtendsParserTest, multipleExtensions) {
    auto itBeginsHere = std::string{R"raw({% extends (extendsBase.file) %}

        {% block nestedInBase %}
        Hello, world!
        {% end_block %}
    )raw"};
    auto command = HtmlCommand{"{% extends (extendsBase.file) %}", 0, 32};
    auto [position, node] = ExtendsParser::parse(itBeginsHere, command);
    auto noDataNeeded = JsonObject{};
    auto result = node->render(noDataNeeded);
    stripAll(result);
    ASSERT_EQ("[[\n-->\nHello,world!\n<--\n]]", result);
}
}