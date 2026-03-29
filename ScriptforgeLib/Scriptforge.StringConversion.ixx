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

export module Scriptforge.StringConversion;

import std;

namespace Scriptforge::StringConversion {
    // ==============================================
   // 概念：判断一个类型是否是 basic_string
   // ==============================================
    template <typename T>
    concept is_basic_string = requires(T str) {
        requires std::same_as<T, std::basic_string<typename T::value_type>>;
    };

    // ==============================================
    // 概念：From 和 To 都必须是 basic_string
    // ==============================================
    template <class ToStr, class FromStr>
    concept convertible_basic_string =
        is_basic_string<FromStr> &&
        is_basic_string<ToStr>;

    // ==============================================
    // 通用转换函数（支持所有 basic_string）
    // ==============================================
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
        else {
            using ToChar = typename ToStr::value_type;
            ToStr result;
            result.reserve(str.size());

            for (auto ch : str) {
                result += static_cast<ToChar>(ch);
            }
            return result;
        }
    }
}