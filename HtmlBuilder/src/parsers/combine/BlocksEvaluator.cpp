#include "cppflask/html/parsers/combine/BlocksEvaluator.h"

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
}

namespace cppflask::html::parsers {

    void BlocksEvaluator::combine(const std::string& blocksDescription, std::string& target) {

        static BlocksEvaluator _instance{};
        _instance.readBlocks(blocksDescription);

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

                auto foundAtleastOne = false;
                auto blockContent = std::string{};
                for(auto configuredBlock = _instance._blocks.find(blockName);
                    configuredBlock != _instance._blocks.end();
                    configuredBlock = _instance._blocks.find(blockName)) {
                        blockContent += configuredBlock->second;
                    _instance._blocks.erase(configuredBlock);
                    foundAtleastOne = true;
                }
                
                if (foundAtleastOne) {
                    target.replace(startPos, commandEndPos + END_BLOCK_CMD.length() - startPos, blockContent);
                    endPos = startPos + blockContent.length();
                }
            }

            startPos = target.find("{%", endPos);
        }
    }

    void BlocksEvaluator::readBlocks(const std::string& html) {
        
        auto startPos = html.find("{%");
        while (startPos != std::string::npos) {
            auto endPos = html.find("%}", startPos);
            if (endPos == std::string::npos) {
                return;
            }

            auto command = html.substr(startPos + 2, endPos - (startPos + 2));
            if (contains(command, BLOCK_CMD)) {
                auto blockName = getBlockName(command, endPos);

                auto nextCommandPos = html.find("{%", endPos + 2);
                auto commandEndPos = find(html, END_BLOCK_CMD, endPos + 2);
                if (commandEndPos == std::string::npos) {
                    return;
                }
                auto nestedBlockCount = 0;
                while (nextCommandPos < commandEndPos || nestedBlockCount != 0) {
                    auto endNextCommandPos = html.find("%}", nextCommandPos + 2);
                    auto nextCommand = html.substr(nextCommandPos, endNextCommandPos + 2 - nextCommandPos);
                    if (contains(nextCommand, BLOCK_CMD)) {
                        nestedBlockCount++;
                    } else if (nextCommand == END_BLOCK_CMD) {
                        nestedBlockCount--;
                        commandEndPos = find(html, END_BLOCK_CMD, endNextCommandPos + 2);
                    }
                    nextCommandPos = html.find("{%", nextCommandPos + 2);
                }

                _blocks.emplace(blockName, html.substr(endPos + 2, commandEndPos - (endPos + 2)));
                endPos = commandEndPos + END_BLOCK_CMD.length();
            }

            startPos = html.find("{%", endPos);
        }
    }
}