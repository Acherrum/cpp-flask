#include "cppflask/html/parsers/combine/BlocksEvaluator.h"

#include <unordered_map>

#include "cppflask/html/StringHelper.h"

using cppflask::html::contains;
using cppflask::html::find;
using cppflask::html::strip;

namespace {
const std::string BLOCK_CMD{" block "};
const std::string END_BLOCK_CMD{"{% end_block %}"};

std::string getBlockName(const std::string& command, size_t commandEndPos) {
    auto nameStartPos = find(command, BLOCK_CMD) + BLOCK_CMD.length();
    auto blockName = command.substr(nameStartPos, commandEndPos - nameStartPos);
    strip(blockName);
    return blockName;
}

std::unordered_map<std::string, std::string> readBlocks(const std::string& html) {
    auto result = std::unordered_map<std::string, std::string>{};
    auto startPos = html.find("{%");
    while (startPos != std::string::npos) {
        auto endPos = html.find("%}", startPos);
        if (endPos == std::string::npos) {
            return {};
        }

        auto command = html.substr(startPos + 2, endPos - (startPos + 2));
        if (contains(command, BLOCK_CMD)) {
            auto blockName = getBlockName(command, endPos);

            auto commandEndPos = find(html, END_BLOCK_CMD, endPos + 2);
            if (commandEndPos == std::string::npos) {
                return {};
            }

             result[blockName] = html.substr(endPos + 2, commandEndPos - (endPos + 2));
            endPos = commandEndPos + END_BLOCK_CMD.length();
        }

        startPos = html.find("{%", endPos);
    }
    return result;
}
}

namespace cppflask::html::parsers {
    void BlocksEvaluator::combine(const std::string& blocksDescription, std::string& target) {

        auto blocks = readBlocks(blocksDescription);

        auto startPos = target.find("{%");
        while (startPos != std::string::npos) {
            auto endPos = target.find("%}", startPos);
            if (endPos == std::string::npos) {
                return;
            }
            auto command = target.substr(startPos + 2, endPos - (startPos + 2));
            if (contains(command, BLOCK_CMD)) {
                auto blockName = getBlockName(command, endPos);

                auto commandEndPos = find(target, END_BLOCK_CMD, endPos + 2);
                if (commandEndPos == std::string::npos) {
                    return;
                }
                auto configuredBlock = blocks.find(blockName);
                if (configuredBlock != blocks.end()) {
                    target.replace(startPos, commandEndPos + END_BLOCK_CMD.length() - startPos, configuredBlock->second);
                    endPos = startPos + configuredBlock->second.length();
                }
            }

            startPos = target.find("{%", endPos);
        }
    }
}