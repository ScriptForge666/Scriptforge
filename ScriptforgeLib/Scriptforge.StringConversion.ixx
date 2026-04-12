// Copyright 2026 Scriptforge
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//     http://www.apache.org/licenses/LICENSE-2.0
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

/**
 * @file Scriptforge.StringConversion.ixx
 * @brief 定义了 `Scriptforge::StringConversion` 模块，其中包含了一个模板函数 `str_convert`，用于在不同字符串类型之间进行转换。该函数使用了 C++20 的 `if constexpr` 语法来根据输入和输出字符串类型的不同，选择合适的转换方法。通过使用这个函数，可以方便地在标准库字符串类型（std::basic_string模板）之间进行转换，提高代码的可读性和可维护性。
 * @author Scriptforge
 * @date 2026/4/1
*/

module;

#include "utf8/utf8/cpp20.h"

export module Scriptforge.StringConversion;

import Scriptforge.Pch;

namespace Scriptforge::StringConversion {
    export
    template <typename T>
    concept is_basic_string = requires(T str) {
        requires std::same_as<T, std::basic_string<typename T::value_type>>;
    };

    template <class ToStr, class FromStr>
    concept convertible_basic_string =
        is_basic_string<FromStr> &&
        is_basic_string<ToStr>;

    export
    template <class ToStr, class FromStr>
    requires convertible_basic_string<ToStr, FromStr>
    ToStr str_convert(const FromStr& str);
}

namespace Scriptforge::StringConversion {
    template <class ToStr, class FromStr>
    requires convertible_basic_string<ToStr, FromStr>
    ToStr str_convert(const FromStr& str) {
        if constexpr (std::is_same_v<ToStr, FromStr>) {
            return str;
        }

        //------------------------------------------------------------------
        // ✨ 任意类型 → string（自动中转 u8）
        //------------------------------------------------------------------
        else if constexpr (std::is_same_v<ToStr, std::string>) {
            return str_convert<std::string, std::u8string>(
                str_convert<std::u8string, FromStr>(str)
            );
        }

        //------------------------------------------------------------------
        // ✨ string → 任意类型（自动中转 u8）
        //------------------------------------------------------------------
        else if constexpr (std::is_same_v<FromStr, std::string>) {
            return str_convert<ToStr, std::u8string>(
                str_convert<std::u8string, std::string>(str)
            );
        }

        //------------------------------------------------------------------
        // u8 ↔ u16
        //------------------------------------------------------------------
        else if constexpr (std::is_same_v<ToStr, std::u16string> &&
            std::is_same_v<FromStr, std::u8string>) {
            return utf8::utf8to16(str);
        }
        else if constexpr (std::is_same_v<ToStr, std::u8string> &&
            std::is_same_v<FromStr, std::u16string>) {
            return utf8::utf16tou8(str);
        }

        //------------------------------------------------------------------
        // u8 ↔ u32
        //------------------------------------------------------------------
        else if constexpr (std::is_same_v<ToStr, std::u32string> &&
            std::is_same_v<FromStr, std::u8string>) {
            return utf8::utf8to32(str);
        }
        else if constexpr (std::is_same_v<ToStr, std::u8string> &&
            std::is_same_v<FromStr, std::u32string>) {
            return utf8::utf32tou8(str);
        }

        //------------------------------------------------------------------
        // string ↔ u8string（底层强转）
        //------------------------------------------------------------------
        else if constexpr (std::is_same_v<ToStr, std::u8string> &&
            std::is_same_v<FromStr, std::string>) {
            return std::u8string(reinterpret_cast<const char8_t*>(str.data()), str.size());
        }
        else if constexpr (std::is_same_v<ToStr, std::string> &&
            std::is_same_v<FromStr, std::u8string>) {
            return std::string(reinterpret_cast<const char*>(str.data()), str.size());
        }

        //------------------------------------------------------------------
        // u8 ↔ wstring（跨平台）
        //------------------------------------------------------------------
        else if constexpr (std::is_same_v<ToStr, std::wstring> &&
            std::is_same_v<FromStr, std::u8string>) {
            if constexpr (sizeof(wchar_t) == 2) {
                auto u16 = utf8::utf8to16(str);
                return std::wstring(reinterpret_cast<const wchar_t*>(u16.data()), u16.size());
            } else {
                auto u32 = utf8::utf8to32(str);
                return std::wstring(reinterpret_cast<const wchar_t*>(u32.data()), u32.size());
            }
        }
        else if constexpr (std::is_same_v<ToStr, std::u8string> &&
            std::is_same_v<FromStr, std::wstring>) {
            if constexpr (sizeof(wchar_t) == 2) {
                std::u16string u16(reinterpret_cast<const char16_t*>(str.data()), str.size());
                return utf8::utf16tou8(u16);
            } else {
                std::u32string u32(reinterpret_cast<const char32_t*>(str.data()), str.size());
                return utf8::utf32tou8(u32);
            }
        }

        //------------------------------------------------------------------
        // ❌ 不支持的组合
        //------------------------------------------------------------------
        else {
            static_assert(
                !convertible_basic_string<ToStr, FromStr>,
                "str_convert: 不支持该字符串类型组合"
            );
        }
    }
}