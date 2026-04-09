#ifndef UTILS_H
#define UTILS_H

#include <emscripten/emscripten.h>
#include <string>
#include <algorithm>
#include <cctype>

extern "C"
{
    void js_alert(const char* str);
}

std::string remove_whitespace_str(const std::string& input);
std::string remove_whitespace_str_from_back(const std::string& s);

#endif