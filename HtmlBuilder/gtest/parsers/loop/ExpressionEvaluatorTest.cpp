#include "gtest/gtest.h"

#include "cppflask/html/parsers/loop/ExpressionEvaluator.h"

namespace cppflask::html::parsers::loop {

class LoopExpressionEvaluatorTest : public testing::Test {};

TEST_F(LoopExpressionEvaluatorTest, classicForExpression) {

    auto result = ExpressionEvaluator::evaluate("i = 0 TO 10");
    ASSERT_EQ(result.start, "0");
    ASSERT_EQ(result.end, "10");
    ASSERT_EQ(result.variable, "i");
    ASSERT_EQ(result.dataVar, "");
}

TEST_F(LoopExpressionEvaluatorTest, forEachExpression) {

    auto result = ExpressionEvaluator::evaluate("element IN $array");
    ASSERT_EQ(result.start, "");
    ASSERT_EQ(result.end, "");
    ASSERT_EQ(result.variable, "element");
    ASSERT_EQ(result.dataVar, "array");
}

TEST_F(LoopExpressionEvaluatorTest, classicForExpression_lessSpacing) {

    auto result = ExpressionEvaluator::evaluate("i=0 TO 10");
    ASSERT_EQ(result.start, "0");
    ASSERT_EQ(result.end, "10");
    ASSERT_EQ(result.variable, "i");
    ASSERT_EQ(result.dataVar, "");
}

TEST_F(LoopExpressionEvaluatorTest, classicForExpression_usingVariables) {

    auto result = ExpressionEvaluator::evaluate("i=$start TO $end");
    ASSERT_EQ(result.start, "$start");
    ASSERT_EQ(result.end, "$end");
    ASSERT_EQ(result.variable, "i");
    ASSERT_EQ(result.dataVar, "");
}


}