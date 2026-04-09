#include "Utils.hpp"

EM_JS(void, js_alert, (const char* str), {
    alert(UTF8ToString(str));
});

std::string remove_whitespace_str(const std::string& input)
{
    std::string output;
    output.reserve(input.size());

    std::copy_if(input.begin(), input.end(), std::back_inserter(output),
        [](unsigned char c)
        {
            return !std::isspace(c);
        });

    return output;
}

std::string remove_whitespace_str_from_back(const std::string& s)
{
    std::string output = s;
    while (!output.empty() && std::isspace(static_cast<unsigned char>(output.back())))
    {
        output.pop_back();
    }

    return output;
}