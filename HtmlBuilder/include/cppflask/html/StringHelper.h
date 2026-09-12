#pragma once

#include <string>
#include <algorithm>

namespace cppflask::html {
inline void stripAll(std::string& input, char character = ' ') {

    std::erase_if(input, [&](const char c) {
        return c == character;
    });
}

inline void strip(std::string& input, char character = ' ') {
    input.erase(input.begin(), std::ranges::find_if(input, [character](const unsigned char ch) {
        return ch != character;
    }));
    input.erase(std::find_if(input.rbegin(), input.rend(), [character](const unsigned char ch) {
        return ch != character;
    }).base(), input.end());
}

inline std::string strip(const std::string& input, const char character = ' ') {
    
    auto copy = input;
    strip(copy, character);
    return copy;
}
}