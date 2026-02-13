export module Scriptforge.Local;

import Scriptforge.Err;
import Scriptforge.ErrCode;
import "json.hpp";
import std;

namespace fs = std::filesystem;
using json = nlohmann::json;

namespace Scriptforge{
    inline namespace Local {
        export class Lang {
        public:
            // 构造函数
            Lang(std::locale loc = std::locale{}, fs::path path = { "lang/" }) : m_loc{ loc }, m_lang_path{ path } {
                loadLanguageFile(path);
            }
            // 重新加载语言文件
            void reload() {
                loadLanguageFile(m_lang_path);
            }
            // 切换到另一个语言
            void setLocale(const std::locale& loc) {
                m_loc = loc;
                loadLanguageFile(m_lang_path);
            }
            // 获取当前语言环境
            std::locale getLocale() const { return m_loc; }

            //获取语言目录
            fs::path getLangPath() const { return m_lang_path; }

            // 设置语言目录
            void setLangPath(const fs::path& path) {
                m_lang_path = path;
                loadLanguageFile(path);
            }

            // 获取语言名称（用于显示）
            std::string getLanguageName() const {
                return j.value("language_name", m_loc.name());
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
                fs::path filename = m_lang_path / (m_loc.name() + ".json");
                std::ifstream jsoninput{ filename };

                if (!jsoninput.is_open()) {
                    throw Scriptforge::Error{
                        Scriptforge::ErrCode::toString(Scriptforge::ErrCode::ErrCode::Local0001),
                        notfound(m_loc)
                    };
                }

                try {
                    jsoninput >> j;
                }
                catch (const json::parse_error& e) {
                    throw Scriptforge::Error{
                        Scriptforge::ErrCode::toString(Scriptforge::ErrCode::ErrCode::Local0001),
                        "Failed to parse language file: " + std::string(e.what())
                    };
                }
            }

            std::string notfound(const std::locale& loc) {
                if (loc.name().find("zh-CN") != std::string::npos)
                    return "无法找到lang/" + loc.name() + ".json";
                else
                    return "cannot find lang/" + loc.name() + ".json";
            }
            std::locale m_loc;
            fs::path m_lang_path;
            json j;
        };
    }
}