#pragma once

#include <string>
#include <algorithm>

namespace cppflask::html {
inline std::size_t rfind_first_not_of(const std::string& input, const std::string& characters) {
    auto end = input.rbegin();
    for (; end != input.rend(); end++) {
        bool hasMatch = false;
        for (const auto& c : characters) {
            if (*end == c) {
                hasMatch = true;
                break;
            }
        }
        if (!hasMatch) {
            break;
        }
    }
    return end == input.rend() ? std::string::npos : input.rend() - end;
}

/**
 * Strip all characters matching 'character' from the input string.
 * @param input
 * @param character
 */
inline void stripAll(std::string& input, char character = ' ') {

    std::erase_if(input, [&](const char c) {
        return c == character;
    });
}

/**
 * Strip all characters matching 'character' from the input string.
 * @param input
 * @param character
 * @returns copy of the input string with all 'character' stripped.
 */
inline std::string stripAll(const std::string& input, char character = ' ') {

    auto copy = input;
    stripAll(copy);
    return copy;
}

/**
 * Trim characters matching 'character' from the beginning and end of the input string.
 * @param input
 * @param character
 */
inline void trim(std::string& input, const std::string& characters = " \n\r\t") {

    input.erase(0, input.find_first_not_of(characters));
    auto endOfText = rfind_first_not_of(input, characters);
    if (endOfText < input.length()) {
        input.erase(endOfText);
    }
}

    /**
     * @see trim(std::string& input, std::String character = " \n\r\t")
     */
inline std::string trim(const std::string& input, const std::string& characters = " \n\r\t") {

    auto copy = input;
    trim(copy, characters);
    return copy;
}

/**
 * Case-insensitive find.
 * @param haystack
 * @param needle
 * @param offset
 * @return
 */
inline std::size_t find(const std::string& haystack, const std::string& needle, std::size_t offset = 0UL) {
    if (offset >= haystack.length()) {
        return std::string::npos;
    }

    auto needleIter = std::search(
        haystack.begin() + offset, haystack.end(), needle.begin(), needle.end(),
        [](const unsigned char c1, const unsigned char c2) {
            return std::toupper(c1) == std::toupper(c2);
    });

    return needleIter == haystack.end() ? std::string::npos : std::distance(haystack.begin(), needleIter);
}

/**
 * Case-insensitve contains
 * @param haystack
 * @param needle
 * @param offset
 * @return
 */
inline bool contains(const std::string& haystack, const std::string& needle, std::size_t offset = 0UL) {

    return find(haystack, needle, offset) != std::string::npos;
}

/**
 * Case-insensitive compare.
 * @param apple
 * @param orange
 * @return
 */
inline bool equal(const std::string& apple, const std::string& orange) {
    return std::ranges::equal(apple, orange,
                              [](const auto& c1, const auto& c2) {
                                  return std::toupper(c1) == std::toupper(c2);
                              }
    );
}
}