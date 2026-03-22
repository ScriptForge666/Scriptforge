// Copyright 2025-2026 Scriptforge
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
 * @file Scriptforge.Err.ixx
 * @brief 定义了 `Scriptforge::Err` 模块，其中包含了一个 `Error` 类，用于表示错误信息。该类提供了构造函数、成员函数以及一个友元函数，用于获取错误信息和错误代码，并支持将错误信息输出到流中。通过使用该类，可以方便地处理和传递错误信息，提高代码的可读性和可维护性。
 * @author Scriptforge
 * @date 2026/3/29
*/

export module Scriptforge.Err;
import std;

export namespace Scriptforge {
    inline namespace Err {
        export class Error {
        public:
            Error() = default;
            Error(std::string_view error);
            Error(std::string_view code, std::string_view error);

            std::string_view what() const;
            std::string_view code() const;

            friend std::ostream& operator<<(std::ostream& os, const Error& err);

        private:
            std::string m_code{ "" };
            std::string m_error{ "Unknown Error" };
        };
        export std::ostream& operator<<(std::ostream& os, const class Error& err);

    }
}

namespace Scriptforge {
    inline namespace Err {
        Error::Error(std::string_view error) : m_error{ error } {}
        Error::Error(std::string_view code, std::string_view error) :m_code{ code }, m_error{ error } {}

        std::string_view Error::what() const { return m_error; }
        std::string_view Error::code() const { return m_code; }

        std::ostream& operator<<(std::ostream& os, const Error& err) {
            os.put('[') << err.code() << ']' << " " << err.what();
            return os;
        }
    }
}