#include "gtest/gtest.h"

#include "cppflask/html/parsers/conditional/ExpressionEvaluator.h"
#include "cppflask/JsonObject.h"

namespace cppflask::html::parsers::conditional {
class ConditionExpressionEvaluatorTest : public testing::Test {};

TEST_F(ConditionExpressionEvaluatorTest, LogicalAnd_TwoBooleans) {

    ASSERT_TRUE(ExpressionEvaluator{"($truth AND $other)"}.evaluate(JsonObject{R"raw({"truth":true,"other":true})raw"}));
}

TEST_F(ConditionExpressionEvaluatorTest, LogicalOr_TwoBooleans) {

    ASSERT_TRUE(ExpressionEvaluator{"($truth OR $other)"}.evaluate(JsonObject{R"raw({"truth":false,"other":true})raw"}));
}

TEST_F(ConditionExpressionEvaluatorTest, AndNestedAnd) {

    ASSERT_TRUE(ExpressionEvaluator{"($truth AND ($truth AND $truth))"}.evaluate(JsonObject{R"raw({"truth":true,"other":false})raw"}));
}

TEST_F(ConditionExpressionEvaluatorTest, AndNestedOr) {

    ASSERT_TRUE(ExpressionEvaluator{"($truth AND ($other OR $other == false))"}.evaluate(JsonObject{R"raw({"truth":true,"other":false})raw"}));
}
}