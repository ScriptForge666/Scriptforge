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
 * @file Scriptforge.Local.ixx
 * @brief 定义了 `Scriptforge::Local` 模块，其中包含了一个 `Lang` 类，用于处理与语言相关的本地化功能。该类包含一个构造函数，接受一个语言枚举值和一个文件路径参数，用于指定语言文件的位置。
 * @author Scriptforge
 * @date 2026/3/29
 */

export module Scriptforge.Local;

import Scriptforge.Err;
import Scriptforge.ErrCode;
import Scriptforge.LanguageCode;
import "json.hpp";
import std;
namespace fs = std::filesystem;
using json = nlohmann::json;

namespace Scriptforge{
    inline namespace Local {
        export class Lang {
        public:
            Lang(Scriptforge::LanguageCode::Language lang{ Scriptforge::LanguageCode::Language::Neutral }, fs::path p{ "/lang" });
        private:
            Scriptforge::LanguageCode::Language m_lang;
			fs::path m_path;
        };
    }
}

namespace Scriptforge {
    inline namespace Local {
        Lang::Lang(Scriptforge::LanguageCode::Language lang, fs::path p):
            m_lang(lang), m_path(std::move(p))
        {
            
		}
    }
}