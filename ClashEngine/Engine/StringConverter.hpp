//Ref:https://stackoverflow.com/questions/31302506/stdu32string-conversion-to-from-stdstring-and-stdu16string

#pragma once

#include <string>
#include <codecvt>

namespace ClashEngine
{
    typedef std::string u8string;

    class StringConverter
    {
    public:
        static u8string To_UTF8(const std::u16string& s)
        {
            std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> conv;
            return conv.to_bytes(s);
        }

        static u8string To_UTF8(const std::u32string& s)
        {
            std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> conv;
            return conv.to_bytes(s);
        }

        static std::u16string To_UTF16(const u8string& s)
        {
            std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> conv;
            return conv.from_bytes(s);
        }

        static std::u16string To_UTF16(const std::u32string& s)
        {
            std::wstring_convert<std::codecvt_utf16<char32_t>, char32_t> conv;
            std::string bytes = conv.to_bytes(s);
            return std::u16string(reinterpret_cast<const char16_t*>(bytes.c_str()), bytes.length() / sizeof(char16_t));
        }

        static std::u32string To_UTF32(const u8string& s)
        {
            //fix: codecvt_utf8 => std::codecvt_utf8
            std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> conv;
            return conv.from_bytes(s);
        }

        static std::u32string To_UTF32(const std::u16string& s)
        {
            const char16_t* pData = s.c_str();
            std::wstring_convert<std::codecvt_utf16<char32_t>, char32_t> conv;
            return conv.from_bytes(reinterpret_cast<const char*>(pData), reinterpret_cast<const char*>(pData + s.length()));
        }
    };
}