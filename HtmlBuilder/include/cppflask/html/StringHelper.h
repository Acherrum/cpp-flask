#pragma once

#include <string>
#include <algorithm>

namespace cppflask::html {
inline void stripAll(std::string& input, char character = ' ') {

    input.erase(std::remove_if(input.begin(), input.end(), [&](char c) {
                     return c == character;
                 }), input.end());
}

inline void strip(std::string& input, char character = ' ') {
    input.erase(input.begin(), std::find_if(input.begin(), input.end(), [character](unsigned char ch) {
        return ch != character;
    }));
    input.erase(std::find_if(input.rbegin(), input.rend(), [character](unsigned char ch) {
        return ch != character;
    }).base(), input.end());
}

inline std::string strip(const std::string& input, char character = ' ') {
    
    auto copy = input;
    strip(copy, character);
    return copy;
}
}