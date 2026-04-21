module;
#include "utf8/utf8/cpp20.h"
export module Scriptforge.StringConversion;
import Scriptforge.Pch;

namespace Scriptforge::StringConversion
{
    export template <typename T>
        concept is_basic_string = requires(T str) {
        requires std::same_as<T, std::basic_string<typename T::value_type>>;
    };

    // ------------------------------
    // 对外唯一接口：str_convert
    // ------------------------------
    export template <class ToStr, class FromStr>
        requires is_basic_string<ToStr>&& is_basic_string<FromStr>
    ToStr str_convert(const FromStr& str);

    // ------------------------------
    // 内部转换实现（内部使用）
    // ------------------------------
    template <class To, class From>
    To convert_string(const From& str);
}

// ----------------------------------------------------------------------------
// 实现：全部拆成独立函数，结构干净到爆炸
// ----------------------------------------------------------------------------
namespace Scriptforge::StringConversion
{
    template <class ToStr, class FromStr>
        requires is_basic_string<ToStr>&& is_basic_string<FromStr>
    ToStr str_convert(const FromStr& str)
    {
        if constexpr (std::is_same_v<ToStr, FromStr>)
            return str;
        else
            return convert_string<ToStr, FromStr>(str);
    }


    // -------------------------------------------------------------------------
    // u8 <-> u16
    // -------------------------------------------------------------------------
    template<>
    std::u16string convert_string<std::u16string, std::u8string>(const std::u8string& str) {
        return utf8::utf8to16(str);
    }

    template<>
    std::u8string convert_string<std::u8string, std::u16string>(const std::u16string& str) {
        return utf8::utf16tou8(str);
    }

    // -------------------------------------------------------------------------
    // u8 <-> u32
    // -------------------------------------------------------------------------
    template<>
    std::u32string convert_string<std::u32string, std::u8string>(const std::u8string& str) {
        return utf8::utf8to32(str);
    }

    template<>
    std::u8string convert_string<std::u8string, std::u32string>(const std::u32string& str) {
        return utf8::utf32tou8(str);
    }

    // -------------------------------------------------------------------------
    // string <-> u8string
    // -------------------------------------------------------------------------
    template<>
    std::u8string convert_string<std::u8string, std::string>(const std::string& str) {
        return std::u8string(reinterpret_cast<const char8_t*>(str.data()), str.size());
    }

    template<>
    std::string convert_string<std::string, std::u8string>(const std::u8string& str) {
        return std::string(reinterpret_cast<const char*>(str.data()), str.size());
    }

    // -------------------------------------------------------------------------
    // u8 <-> wstring
    // -------------------------------------------------------------------------
    template<>
    std::wstring convert_string<std::wstring, std::u8string>(const std::u8string& str)
    {
        if constexpr (sizeof(wchar_t) == 2) {
            auto u16 = utf8::utf8to16(str);
            return std::wstring(reinterpret_cast<const wchar_t*>(u16.data()), u16.size());
        }
        else {
            auto u32 = utf8::utf8to32(str);
            return std::wstring(reinterpret_cast<const wchar_t*>(u32.data()), u32.size());
        }
    }

    template<>
    std::u8string convert_string<std::u8string, std::wstring>(const std::wstring& str)
    {
        if constexpr (sizeof(wchar_t) == 2) {
            std::u16string u16(reinterpret_cast<const char16_t*>(str.data()), str.size());
            return utf8::utf16tou8(u16);
        }
        else {
            std::u32string u32(reinterpret_cast<const char32_t*>(str.data()), str.size());
            return utf8::utf32tou8(u32);
        }
    }

    // ==============================
// 万能中转：任意 → string ✅ 合法
// ==============================
    template <is_basic_string From>
        requires (!std::is_same_v<From, std::string>)
    std::string convert_string(const From& str)
    {
        return str_convert<std::string>(str_convert<std::u8string>(str));
    }

    // ==============================
    // 万能中转：任意 → wstring ✅ 合法
    // ==============================
    template <is_basic_string From>
        requires (!std::is_same_v<From, std::wstring>)
    std::wstring convert_string(const From& str)
    {
        return str_convert<std::wstring>(str_convert<std::u8string>(str));
    }

    // ==============================
    // 万能中转：string → 任意 ✅ 合法
    // ==============================
    template <is_basic_string To>
        requires (!std::is_same_v<To, std::string>)
    To convert_string(const std::string& str)
    {
        return str_convert<To>(str_convert<std::u8string>(str));
    }
}