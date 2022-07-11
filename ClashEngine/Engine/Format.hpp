#ifndef CLASH_FORMAT_HPP
#define CLASH_FORMAT_HPP

#include <sstream>
#include <string>
#include <string_view>

//Ref:https://codereview.stackexchange.com/questions/269425/implementing-stdformat

//Note:
//https://stackoverflow.com/questions/58295334/error-stdstring-view-has-not-been-declared
//std::string_view is only available in c++17 or newer.
//For g++, use the switch -std=c++17

//About std::format:https://zhuanlan.zhihu.com/p/355166681

namespace ClashEngine
{
    class Format
    {
    private:
        template<typename T>
        static void format_helper(std::ostringstream& oss, std::string_view& str, const T& value)
        {
            std::size_t openBracket = str.find('{');
            if (openBracket == std::string::npos)
            {
                return;
            }
            std::size_t closeBracket = str.find('}', openBracket + 1);
            if (closeBracket == std::string::npos)
            {
                return;
            }
            oss << str.substr(0, openBracket) << value;
            str = str.substr(closeBracket + 1);
        }

    public:
        template<typename... Targs>
        static std::string format(std::string_view str, Targs...args)
        {
            std::ostringstream oss;
            (format_helper(oss, str, args), ...);
            oss << str;
            return oss.str();
        }
    };
}

#endif