#pragma once

#include <string>
#include <algorithm>

namespace cppflask::html {
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
 * Strip characters matching 'character' from the beginning and end of the input string.
 * @param input
 * @param character
 */
inline void strip(std::string& input, char character = ' ') {
    input.erase(input.begin(), std::ranges::find_if(input, [character](const unsigned char ch) {
        return ch != character;
    }));
    input.erase(std::find_if(input.rbegin(), input.rend(), [character](const unsigned char ch) {
        return ch != character;
    }).base(), input.end());
}

/**
 * @see strip(std::string& input, char character = ' ')
 */
inline std::string strip(const std::string& input, const char character = ' ') {
    
    auto copy = input;
    strip(copy, character);
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