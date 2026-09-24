#include "gtest/gtest.h"

#include "cppflask/html/parsers/combine/BlocksEvaluator.h"

namespace cppflask::html::parsers {

class BlocksEvaluatorTest : public testing::Test {};

TEST_F(BlocksEvaluatorTest, blocksAreCombinedInTarget) {
    auto target = std::string{"-->{% block replace %}{% end_block %}<--"};
    auto definition = std::string{"{% block replace %}Hello, world!{% end_block %}"};
    BlocksEvaluator::combine(definition, target);
    ASSERT_EQ("-->Hello, world!<--", target);
}

TEST_F(BlocksEvaluatorTest, blockCommandsRemainIfTheyDoNotExist) {
    auto target = std::string{"-->{% block notReplace %}{% end_block %}<--"};
    auto definition = std::string{"{% block replace %}Hello, world!{% end_block %}"};
    BlocksEvaluator::combine(definition, target);
    ASSERT_EQ("-->{% block notReplace %}{% end_block %}<--", target);
}

TEST_F(BlocksEvaluatorTest, multipleBlocksCanBeEvaluated) {
    auto target = std::string{"-->{% block hello %}{% end_block %}, {% block world %}{% end_block %}!<--"};
    auto definition = std::string{"{% block hello %}HeLLo{% end_block %}{% block world %}WORLD{% end_block %}"};
    BlocksEvaluator::combine(definition, target);
    ASSERT_EQ("-->HeLLo, WORLD!<--", target);
}
}