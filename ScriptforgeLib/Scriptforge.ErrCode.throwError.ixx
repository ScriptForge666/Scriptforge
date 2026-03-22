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
 * @file Scriptforge.ErrCode.ixx
 * @brief 定义了 `Scriptforge::ErrCode` 模块，其中包含一个throwError函数的实现，以保证不出现循环依赖。该函数接受一个错误代码、函数名称、语言对象和参数列表，并根据提供的信息构建错误消息，然后抛出一个包含错误代码和消息的异常。通过使用该函数，可以方便地处理和传递错误信息，提高代码的可读性和可维护性。
 * @author Scriptforge
 * @date 2026/3/29
 */

module Scriptforge.ErrCode;
import Scriptforge.ErrCode;
import Scriptforge.Local;
import Scriptforge.Err;
import std;


namespace Scriptforge::ErrCode {
    void throwError(
        ErrCode code,
        const std::string& func,
        const Scriptforge::Local::Lang& lang,
        const std::vector <std::string> args
    ) {

        Scriptforge::Local::Lang default_en_lang{ "en", lang.getLangPath() };

        std::string default_en_msg = default_en_lang.get(std::to_string(static_cast<int>(code)), toString(code));

        std::string base_msg;
        
		if (args.empty()) {
            base_msg = lang.get(std::to_string(static_cast<int>(code)), default_en_msg);
        } else {
            base_msg = lang.format(std::to_string(static_cast<int>(code)), args);
            if (base_msg == std::to_string(static_cast<int>(code))) {
                base_msg = default_en_lang.format(std::to_string(static_cast<int>(code)), args);
            }
        }

        std::string full_msg = func + ":" + base_msg;

        throw Scriptforge::Error{ toString(code), full_msg };
    }
}