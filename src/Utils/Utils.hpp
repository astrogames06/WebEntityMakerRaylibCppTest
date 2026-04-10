#ifndef UTILS_H
#define UTILS_H

#include <emscripten/emscripten.h>
#include <string>
#include <algorithm>
#include <cctype>
#include <vector>
#include <sstream>

extern "C"
{
    void js_alert(const char* str);
}

std::string remove_whitespace_str(const std::string& input);
std::string remove_whitespace_str_from_back(const std::string& s);
std::vector<std::string> split_str(const std::string& s, char delim);

#endif