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

import Scriptforge.StringConversion;
import Scriptforge.Err;
import Scriptforge.ErrCode;
import Scriptforge.LanguageCode;
import "json.hpp";
import std;
namespace fs = std::filesystem;
using json = nlohmann::json;
using namespace Scriptforge::StringConversion;

namespace Scriptforge {
    inline namespace Local {

        [[noreturn]] void throwFileNotFoundError(const fs::path& p, Scriptforge::LanguageCode::Language lang);
        [[noreturn]] void throwInvalidFileError(const fs::path& p, Scriptforge::LanguageCode::Language lang);

        export class Lang {
        public:
            Lang(Scriptforge::LanguageCode::Language lang{ Scriptforge::LanguageCode::Language::Neutral }, fs::path p{ "/lang" });
            void reload();
            void setLocale(const Scriptforge::LanguageCode::Language lang);
            Scriptforge::LanguageCode::Language getLanguageCode() const;
            template<typename T>
            T getLanguageName() const;
        private:
            void loadLanguageFile(Scriptforge::LanguageCode::Language lang, fs::path path);
            void isLegal() const;
            Scriptforge::LanguageCode::Language m_lang;
            json j;
            fs::path m_path;
        };



    }
}

namespace Scriptforge {
    inline namespace Local {
        [[noreturn]] void throwFileNotFoundError(const fs::path& p, Scriptforge::LanguageCode::Language lang) {
            if (lang == Scriptforge::LanguageCode::Language::Chinese) {
                throw Scriptforge::Error{ Scriptforge::ErrCode::LocalLanguageFileNotFound, "指定的语言文件不存在: " + p.string() };
            }
            else {
                throw Scriptforge::Error{ Scriptforge::ErrCode::LocalLanguageFileNotFound, "The specified language file does not exist: " + p.string() };
            }
        }
        [[noreturn]] void throwInvalidFileError(const fs::path& p, Scriptforge::LanguageCode::Language lang) {
            if (lang == Scriptforge::LanguageCode::Language::Chinese) {
                throw Scriptforge::Error{ Scriptforge::ErrCode::ErrCode::LocalInvalidLanguageFile, "指定的语言文件无效: " + p.string() };
            }
            else {
                throw Scriptforge::Error{ Scriptforge::ErrCode::ErrCode::LocalInvalidLanguageFile, "The specified language file is invalid: " + p.string() };
            }
        }
        Lang::Lang(Scriptforge::LanguageCode::Language lang, fs::path p) {
            loadLanguageFile(lang, p);
        }
        void Lang::reload() {
            loadLanguageFile(m_lang, m_path);
        }
        void Lang::setLocale(const Scriptforge::LanguageCode::Language lang) {
            m_lang = lang;
            loadLanguageFile(lang, m_path);
        }
        Scriptforge::LanguageCode::Language Lang::getLanguageCode() const {
            return m_lang;
        }
        template<typename T>
        T Lang::getLanguageName() const {
            return str_convert<T>(j.value("language_name", Scriptforge::LanguageCode::ENUM_TO_ISO639_1.get(m_lang)));
        }
        void Lang::loadLanguageFile(Scriptforge::LanguageCode::Language lang, fs::path path) {
            fs::path filename = path / (getLanguageName(lang) + ".json");
            m_lang = lang;
            std::ifstream jsoninput{ filename };
            m_path = filename;

            if (!jsoninput.is_open()) {
                throwFileNotFoundError(filename, lang);
            }
            try {
                jsoninput >> j;
            }
            catch (const json::parse_error&) {
                throwFileNotFoundError(filename, lang);
            }
        }
        void Lang::isLegal() const {
            if (m_lang == Scriptforge::LanguageCode::Language::Neutral || m_lang == Scriptforge::LanguageCode::Language::Invariant) {
                throw Scriptforge::Error{ Scriptforge::ErrCode::ErrCode::LocalInvalidLanguageFile, "Neutral and Invariant languages are not valid for localization." };
            }
            if (!fs::exists(m_path)) {
                throwFileNotFoundError(m_path, m_lang);
            }
            if (!fs::is_regular_file(m_path)) {
                throwInvalidFileError(m_path, m_lang);
            }
        }

    }
}