#include "gtest/gtest.h"

#include "cppflask/html/HtmlBuilder.h"
#include "cppflask/html/StringHelper.h"
#include "cppflask/JsonObject.h"

namespace cppflask::html {
class HtmlBuilderTest : public testing::Test {};

TEST_F(HtmlBuilderTest, BuildSimplyReturnsTheText) {

    auto input = std::string{"{{ $hello }}"};
    auto result = HtmlBuilder::fromText(input)
                    .build();
    ASSERT_EQ(result, "");
}

TEST_F(HtmlBuilderTest, InputIsUnchangedIfVariableIsNotPartOfDataSet) {

    auto input = std::string{"{{ $hello }}"};
    auto data = JsonObject(R"raw({"nope":"world"})raw");
    auto result = HtmlBuilder::fromText(input)
                    .buildWithData(data);
    ASSERT_EQ(result, "");
}

TEST_F(HtmlBuilderTest, BuildWithDataReplacesVariablesWhenInDataSet) {

    auto input = std::string{"{{ $hello }}"};
    auto data = JsonObject(R"raw({"hello":"world"})raw");
    auto result = HtmlBuilder::fromText(input)
                    .buildWithData(data);
    ASSERT_EQ(result, "world");
}

TEST_F(HtmlBuilderTest, BuildFromFileAndConditionalFormatting_0_2_7) {

    auto data = JsonObject(R"raw({"input":[true, false, true, false, false, false, false, true], "output":["0", "1", "2", "3", "4", "5", "6", "7", "8", "9"]})raw");
    auto result = HtmlBuilder::fromFile("testConditions.file")
                      .buildWithData(data);
    stripAll(result);
    ASSERT_EQ(result, "029");
}

TEST_F(HtmlBuilderTest, BuildFromFileAndConditionalFormatting_None) {

    auto data = JsonObject(R"raw({"input":[false, true, true, false, true, false, false, false], "output":["0", "1", "2", "3", "4", "5", "6", "7", "8", "9"]})raw");
    auto result = HtmlBuilder::fromFile("testConditions.file")
                      .buildWithData(data);
    stripAll(result);
    ASSERT_EQ(result, "");
}

TEST_F(HtmlBuilderTest, BuildFromFileAndConditionalFormatting_0_1_4_5_6) {

    auto data = JsonObject(R"raw({"input":[true, true, false, false, true, true, true, false], "output":["0", "1", "2", "3", 4, "5", 6.0, "7", true, "9"]})raw");
    auto result = HtmlBuilder::fromFile("testConditions.file")
                      .buildWithData(data);
    stripAll(result);
    ASSERT_EQ(result, "01357");
}

TEST_F(HtmlBuilderTest, BuildFromFileAndConditionalFormatting_0_5) {

    auto data = JsonObject(R"raw({"input":[true, false, false, false, false, true, false, false], "output":["0", "1", "2", "3", 4, "5", 6.0, "7", true, "9"]})raw");
    auto result = HtmlBuilder::fromFile("testConditions.file")
                      .buildWithData(data);
    stripAll(result);
    ASSERT_EQ(result, "036.000000true");
}

TEST_F(HtmlBuilderTest, LoopTest) {

    auto data = JsonObject(R"raw({"array":[{"name":"luuk","age":39}, {"name":"laura","age":37}]})raw");
    auto result = HtmlBuilder::fromFile("testLoops.file")
                      .buildWithData(data);
    stripAll(result);
    ASSERT_EQ(result, "[0,0][0,1][0,2][1,0][1,1][1,2][2,0][2,1][2,2]luukis39<br/>laurais37<br/>");
}

TEST_F(HtmlBuilderTest, LoopTest_ClassicWithVariables) {

    auto data = JsonObject(R"raw({"form":{"start":3,"end":7}})raw");
    auto result = HtmlBuilder::fromText("{% FOR (i = $form/start TO $form/end) %}{{ $i }} {% END_FOR %}")
                      .buildWithData(data);
    ASSERT_EQ(result, "3 4 5 6 ");
}

TEST_F(HtmlBuilderTest, OneBuilder_DifferentDataCalls) {

    auto builder = HtmlBuilder::fromText("{% FOR (i = $form/start TO $form/end) %}{{ $i }} {% IF ($i == 0) %}<br />{% END_IF %}{% END_FOR %}");
    {
        auto data = JsonObject(R"raw({"form":{"start":3,"end":7}})raw");
        auto result = builder.buildWithData(data);
        ASSERT_EQ(result, "3 4 5 6 ");
    }
    {
        auto data = JsonObject(R"raw({"form":{"start":-3,"end":2}})raw");
        auto result = builder.buildWithData(data);
        ASSERT_EQ(result, "-3 -2 -1 0 <br />1 ");
    }
}

TEST_F(HtmlBuilderTest, CaseInsensitiveTest) {

    auto builder = HtmlBuilder::fromFile("caseInsensitiveTest.file");
    auto result = builder.build();
    stripAll(result, '\n');
    ASSERT_EQ(result, "neither1 is odd2 is even3 is odd4 is even5 is odd");
}

TEST_F(HtmlBuilderTest, CaseInsensitiveTest_BothTruePath) {

    auto builder = HtmlBuilder::fromFile("caseInsensitiveTest.file");
    auto data = JsonObject(R"raw({"var":true,"VAR":true})raw");
    auto result = builder.buildWithData(data);
    stripAll(result, '\n');
    ASSERT_EQ(result, "both1 is odd2 is even3 is odd4 is even5 is odd");
}
}