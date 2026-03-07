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
            // 构造函数
            Lang(std::string loc = {"en"}, fs::path path = {"lang/"}) : m_lang_path{path} {
				m_loc = getLanguageId(loc);
                loadLanguageFile(path);
            }
            // 重新加载语言文件
            void reload() {
                loadLanguageFile(m_lang_path);
            }
            // 切换到另一个语言
            void setLocale(const std::string loc) {
                m_loc = getLanguageId(loc);
                loadLanguageFile(m_lang_path);
            }
            // 获取当前语言环境
            int getLocaleId() const { return m_loc; }

            //获取语言目录
            fs::path getLangPath() const { return m_lang_path; }

            // 设置语言目录
            void setLangPath(const fs::path& path) {
                m_lang_path = path;
                loadLanguageFile(path);
            }

            // 获取语言名称（用于显示）
            std::string getLanguageNameL() const {
                return j.value("language_name", getLanguageName(m_loc));
            }

            std::string getLanguageName() const {
				return getLanguageName(m_loc);
            }
           


            // 基本文本获取
            std::string get(const std::string& key) const {
                return j.value(key, key); // 默认返回key本身
            }
            // 带默认值的文本获取
            std::string get(const std::string& key, const std::string& defaultValue) const {
                return j.value(key, defaultValue);
            }
            // 格式化文本（支持替换占位符）
            std::string format(const std::string& key, const std::vector<std::string>& args) const {
                std::string text = get(key);

                for (size_t i = 0; i < args.size(); ++i) {
                    std::string placeholder = "{" + std::to_string(i) + "}";
                    size_t pos = text.find(placeholder);
                    if (pos != std::string::npos) {
                        text.replace(pos, placeholder.length(), args[i]);
                    }
                }

                return text;
            }
            // 检查键是否存在
            bool has(const std::string& key) const {
                return j.contains(key);
            }
            // 获取所有可用键
            std::vector<std::string> getKeys() const {
                std::vector<std::string> keys;
                for (auto& element : j.items()) {
                    keys.push_back(element.key());
                }
                return keys;
            }
            // 获取JSON对象（用于高级操作）
            const json& getJson() const { return j; }

            // 检查语言文件是否加载成功
            bool isLoaded() const { return !j.empty(); }

            // 获取可用语言列表（从文件系统）
            static std::vector<std::string> getAvailableLanguages(const std::string& path = "lang/") {
                std::vector<std::string> languages;

                for (const auto& entry : std::filesystem::directory_iterator(path)) {
                    if (entry.path().extension() == ".json") {
                        languages.push_back(entry.path().stem().string());
                    }
                }

                return languages;
            }

        private:
            void loadLanguageFile(fs::path path) {
                fs::path filename = m_lang_path / (getLanguageName(m_loc) + ".json");
                std::ifstream jsoninput{ filename };

                if (!jsoninput.is_open()) {
                    throw Scriptforge::Error{
                        Scriptforge::ErrCode::toString(Scriptforge::ErrCode::ErrCode::LocalLanguageFileNotFound),
                        notfound(m_loc, m_lang_path)
                    };
                }

                try {
                    jsoninput >> j;
                }
                catch (const json::parse_error& e) {
                    throw Scriptforge::Error{
                        Scriptforge::ErrCode::toString(Scriptforge::ErrCode::ErrCode::LocalLanguageFileNotFound),
                        notParseFile(m_loc)
                    };
                }
            }

            int getLanguageId(const std::string& loc) const {
                auto it = LanguageCode::ISO639_1_TO_ID.find(loc);
                if (it != LanguageCode::ISO639_1_TO_ID.end()) {
                    return it->second;
                }
                else {
                    throw Scriptforge::Error{
                        Scriptforge::ErrCode::toString(Scriptforge::ErrCode::ErrCode::LocalInvalidLanguageCode),
                        invalidLanguageName(loc)
                    };
                }
            }

            std::string getLanguageName(int id) const {
                auto it = LanguageCode::ID_TO_ISO639_1.find(id);
                if (it != LanguageCode::ID_TO_ISO639_1.end()) {
                    return it->second;
                }
                else {
                    throw Scriptforge::Error{
                        Scriptforge::ErrCode::toString(Scriptforge::ErrCode::ErrCode::LocalInvalidLanguageCode),
                        invalidLanguageId(id)
                    };
                }
            }

            std::string notfound(const int loc, const fs::path path) const {
                if (loc == getLanguageId("zh"))
                    return "无法找到" + getLanguageName(loc) + ".json";
                else
                    return "cannot find " + getLanguageName(loc) + ".json";
            }

            std::string notParseFile(const int loc) const {
                if (loc == getLanguageId("zh"))
                    return "无法解析语言文件: " + getLanguageName(loc) + ".json";
                else
                    return "Failed to parse language file: " + getLanguageName(loc) + ".json";
            }

            std::string invalidLanguageId(const int& loc) const {
                return "Invalid language code: " + loc;
			}

            std::string invalidLanguageName(const std::string& name) const {
                return "Invalid language ID: " + name;
			}

            int m_loc;
            fs::path m_lang_path;
            json j;
        };
    }
}