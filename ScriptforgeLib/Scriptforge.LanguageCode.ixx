export module Scriptforge.LanguageCode;

import std;

namespace Scriptforge::LanguageCode {
    /* -------------------------------------------------------------
     *  1) 语言宏名 → LCID (十六进制)
     * ------------------------------------------------------------- */
    export inline const std::map<std::string, int> LANG_NAME_TO_ID = {
        {"LANG_NEUTRAL",                     0x00},
        {"LANG_INVARIANT",                   0x7f},

        {"LANG_AFRIKAANS",                   0x36},
        {"LANG_ALBANIAN",                    0x1c},
        {"LANG_ALSATIAN",                    0x84},
        {"LANG_AMHARIC",                     0x5e},
        {"LANG_ARABIC",                      0x01},
        {"LANG_ARMENIAN",                    0x2b},
        {"LANG_ASSAMESE",                    0x4d},
        {"LANG_AZERBAIJANI",                 0x2c},
        {"LANG_BANGLA",                      0x45},
        {"LANG_BASQUE",                      0x2d},
        {"LANG_BELARUSIAN",                  0x23},
        {"LANG_BULGARIAN",                   0x02},
        {"LANG_CATALAN",                     0x03},
        {"LANG_CHEROKEE",                    0x5c},
        {"LANG_CHINESE",                     0x04},
        {"LANG_CORSICAN",                    0x83},
        {"LANG_CROATIAN",                    0x1a},
        {"LANG_CZECH",                       0x05},
        {"LANG_DANISH",                      0x06},
        {"LANG_DUTCH",                       0x13},
        {"LANG_ENGLISH",                     0x09},
        {"LANG_ESTONIAN",                    0x25},
        {"LANG_FAEROESE",                    0x38},
        {"LANG_FINNISH",                     0x0b},  // 添加芬兰语
        {"LANG_FRENCH",                      0x0c},
        {"LANG_FRISIAN",                     0x62},
        {"LANG_FULAH",                       0x67},
        {"LANG_GALICIAN",                    0x56},
        {"LANG_GEORGIAN",                    0x37},
        {"LANG_GERMAN",                      0x07},
        {"LANG_GREEK",                       0x08},
        {"LANG_GUJARATI",                    0x47},
        {"LANG_HEBREW",                      0x0d},
        {"LANG_HINDI",                       0x39},
        {"LANG_HUNGARIAN",                   0x0e},
        {"LANG_ICELANDIC",                   0x0f},
        {"LANG_INDONESIAN",                  0x21},
        {"LANG_IRISH",                       0x3c},
        {"LANG_ITALIAN",                     0x10},
        {"LANG_JAPANESE",                    0x11},
        {"LANG_KANNADA",                     0x4b},
        {"LANG_KAZAK",                       0x3f},
        {"LANG_KHMER",                       0x53},
        {"LANG_KOREAN",                      0x12},
        {"LANG_KYRGYZ",                      0x40},
        {"LANG_LATVIAN",                     0x26},
        {"LANG_LITHUANIAN",                  0x27},
        {"LANG_LOWER_SORBIAN",               0x2e},
        {"LANG_LUXEMBOURGISH",               0x6e},
        {"LANG_MACEDONIAN",                  0x2f},
        {"LANG_MALAY",                       0x3e},
        {"LANG_MALAYALAM",                   0x4c},
        {"LANG_MALTESE",                     0x3a},
        {"LANG_MARATHI",                     0x4e},
        {"LANG_MONGOLIAN",                   0x50},
        {"LANG_NEPALI",                      0x61},
        {"LANG_NORWEGIAN",                   0x14},
        {"LANG_OCCITAN",                     0x82},
        {"LANG_ODIA",                        0x48},
        {"LANG_PERSIAN",                     0x29},
        {"LANG_POLISH",                      0x15},
        {"LANG_PORTUGUESE",                  0x16},
        {"LANG_PUNJABI",                     0x46},
        {"LANG_ROMANIAN",                    0x18},
        {"LANG_RUSSIAN",                     0x19},
        {"LANG_SAMI",                        0x3b},
        {"LANG_SERBIAN",                     0x1a},  // 添加塞尔维亚语，注意与克罗地亚语相同LCID
        {"LANG_SLOVAK",                      0x1b},
        {"LANG_SLOVENIAN",                   0x24},
        {"LANG_SPANISH",                     0x0a},
        {"LANG_SWAHILI",                     0x41},
        {"LANG_SWEDISH",                     0x1d},
        {"LANG_TAMIL",                       0x49},
        {"LANG_TATAR",                       0x44},
        {"LANG_TELUGU",                      0x4a},
        {"LANG_THAI",                        0x1e},
        {"LANG_TURKISH",                     0x1f},
        {"LANG_UKRAINIAN",                   0x22},
        {"LANG_URDU",                        0x20},
        {"LANG_UZBEK",                       0x43},
        {"LANG_VIETNAMESE",                  0x2a},
        {"LANG_WELSH",                       0x52},
        {"LANG_XHOSA",                       0x34},
        {"LANG_YI",                          0x78},
        {"LANG_YORUBA",                      0x6a},
        {"LANG_ZULU",                        0x35}
    };

    /* -------------------------------------------------------------
     *  2) ISO‑639‑1 两字母代码 → LCID
     *  （注意：由于LCID中塞尔维亚语和克罗地亚语使用相同主语言ID，这里可能需要特殊处理）
     * ------------------------------------------------------------- */
    export inline const std::map<std::string, int> ISO639_1_TO_ID = {
        {"af", LANG_NAME_TO_ID.at("LANG_AFRIKAANS")},       // Afrikaans
        {"sq", LANG_NAME_TO_ID.at("LANG_ALBANIAN")},        // Albanian
        {"am", LANG_NAME_TO_ID.at("LANG_AMHARIC")},         // Amharic
        {"ar", LANG_NAME_TO_ID.at("LANG_ARABIC")},          // Arabic
        {"hy", LANG_NAME_TO_ID.at("LANG_ARMENIAN")},        // Armenian
        {"as", LANG_NAME_TO_ID.at("LANG_ASSAMESE")},        // Assamese
        {"az", LANG_NAME_TO_ID.at("LANG_AZERBAIJANI")},     // Azerbaijani
        {"eu", LANG_NAME_TO_ID.at("LANG_BASQUE")},          // Basque
        {"be", LANG_NAME_TO_ID.at("LANG_BELARUSIAN")},      // Belarusian
        {"bg", LANG_NAME_TO_ID.at("LANG_BULGARIAN")},       // Bulgarian
        {"ca", LANG_NAME_TO_ID.at("LANG_CATALAN")},         // Catalan
        {"zh", LANG_NAME_TO_ID.at("LANG_CHINESE")},         // Chinese
        {"hr", LANG_NAME_TO_ID.at("LANG_CROATIAN")},        // Croatian
        {"cs", LANG_NAME_TO_ID.at("LANG_CZECH")},           // Czech
        {"da", LANG_NAME_TO_ID.at("LANG_DANISH")},          // Danish
        {"nl", LANG_NAME_TO_ID.at("LANG_DUTCH")},           // Dutch
        {"en", LANG_NAME_TO_ID.at("LANG_ENGLISH")},         // English
        {"et", LANG_NAME_TO_ID.at("LANG_ESTONIAN")},        // Estonian
        {"fo", LANG_NAME_TO_ID.at("LANG_FAEROESE")},        // Faroese
        {"fi", LANG_NAME_TO_ID.at("LANG_FINNISH")},         // Finnish
        {"fr", LANG_NAME_TO_ID.at("LANG_FRENCH")},          // French
        {"fy", LANG_NAME_TO_ID.at("LANG_FRISIAN")},         // Frisian
        {"gl", LANG_NAME_TO_ID.at("LANG_GALICIAN")},        // Galician
        {"ka", LANG_NAME_TO_ID.at("LANG_GEORGIAN")},        // Georgian
        {"de", LANG_NAME_TO_ID.at("LANG_GERMAN")},          // German
        {"el", LANG_NAME_TO_ID.at("LANG_GREEK")},           // Greek
        {"gu", LANG_NAME_TO_ID.at("LANG_GUJARATI")},        // Gujarati
        {"he", LANG_NAME_TO_ID.at("LANG_HEBREW")},          // Hebrew
        {"hi", LANG_NAME_TO_ID.at("LANG_HINDI")},           // Hindi
        {"hu", LANG_NAME_TO_ID.at("LANG_HUNGARIAN")},       // Hungarian
        {"is", LANG_NAME_TO_ID.at("LANG_ICELANDIC")},       // Icelandic
        {"id", LANG_NAME_TO_ID.at("LANG_INDONESIAN")},      // Indonesian
        {"ga", LANG_NAME_TO_ID.at("LANG_IRISH")},           // Irish
        {"it", LANG_NAME_TO_ID.at("LANG_ITALIAN")},         // Italian
        {"ja", LANG_NAME_TO_ID.at("LANG_JAPANESE")},        // Japanese
        {"kn", LANG_NAME_TO_ID.at("LANG_KANNADA")},         // Kannada
        {"kk", LANG_NAME_TO_ID.at("LANG_KAZAK")},           // Kazakh
        {"km", LANG_NAME_TO_ID.at("LANG_KHMER")},           // Khmer
        {"ko", LANG_NAME_TO_ID.at("LANG_KOREAN")},          // Korean
        {"ky", LANG_NAME_TO_ID.at("LANG_KYRGYZ")},          // Kyrgyz
        {"lv", LANG_NAME_TO_ID.at("LANG_LATVIAN")},         // Latvian
        {"lt", LANG_NAME_TO_ID.at("LANG_LITHUANIAN")},      // Lithuanian
        {"mk", LANG_NAME_TO_ID.at("LANG_MACEDONIAN")},      // Macedonian
        {"ms", LANG_NAME_TO_ID.at("LANG_MALAY")},           // Malay
        {"ml", LANG_NAME_TO_ID.at("LANG_MALAYALAM")},       // Malayalam
        {"mt", LANG_NAME_TO_ID.at("LANG_MALTESE")},         // Maltese
        {"mr", LANG_NAME_TO_ID.at("LANG_MARATHI")},         // Marathi
        {"mn", LANG_NAME_TO_ID.at("LANG_MONGOLIAN")},       // Mongolian
        {"ne", LANG_NAME_TO_ID.at("LANG_NEPALI")},          // Nepali
        {"no", LANG_NAME_TO_ID.at("LANG_NORWEGIAN")},       // Norwegian
        {"fa", LANG_NAME_TO_ID.at("LANG_PERSIAN")},         // Persian (Farsi)
        {"pl", LANG_NAME_TO_ID.at("LANG_POLISH")},          // Polish
        {"pt", LANG_NAME_TO_ID.at("LANG_PORTUGUESE")},      // Portuguese
        {"pa", LANG_NAME_TO_ID.at("LANG_PUNJABI")},         // Punjabi
        {"ro", LANG_NAME_TO_ID.at("LANG_ROMANIAN")},        // Romanian
        {"ru", LANG_NAME_TO_ID.at("LANG_RUSSIAN")},         // Russian
        {"sr", LANG_NAME_TO_ID.at("LANG_SERBIAN")},         // Serbian
        {"sk", LANG_NAME_TO_ID.at("LANG_SLOVAK")},          // Slovak
        {"sl", LANG_NAME_TO_ID.at("LANG_SLOVENIAN")},       // Slovenian
        {"es", LANG_NAME_TO_ID.at("LANG_SPANISH")},         // Spanish
        {"sw", LANG_NAME_TO_ID.at("LANG_SWAHILI")},         // Swahili
        {"sv", LANG_NAME_TO_ID.at("LANG_SWEDISH")},         // Swedish
        {"ta", LANG_NAME_TO_ID.at("LANG_TAMIL")},           // Tamil
        {"tt", LANG_NAME_TO_ID.at("LANG_TATAR")},           // Tatar
        {"te", LANG_NAME_TO_ID.at("LANG_TELUGU")},          // Telugu
        {"th", LANG_NAME_TO_ID.at("LANG_THAI")},            // Thai
        {"tr", LANG_NAME_TO_ID.at("LANG_TURKISH")},         // Turkish
        {"uk", LANG_NAME_TO_ID.at("LANG_UKRAINIAN")},       // Ukrainian
        {"ur", LANG_NAME_TO_ID.at("LANG_URDU")},            // Urdu
        {"uz", LANG_NAME_TO_ID.at("LANG_UZBEK")},           // Uzbek
        {"vi", LANG_NAME_TO_ID.at("LANG_VIETNAMESE")},      // Vietnamese
        {"cy", LANG_NAME_TO_ID.at("LANG_WELSH")},           // Welsh
        {"xh", LANG_NAME_TO_ID.at("LANG_XHOSA")},           // Xhosa
        {"yi", LANG_NAME_TO_ID.at("LANG_YI")},              // Yiddish
        {"yo", LANG_NAME_TO_ID.at("LANG_YORUBA")},          // Yoruba
        {"zu", LANG_NAME_TO_ID.at("LANG_ZULU")}             // Zulu
    };

    /* -------------------------------------------------------------
     *  3) （可选）LCID → ISO‑639‑1 反向映射
     *  注意：由于多个语言可能共享同一个LCID主ID（如塞尔维亚语和克罗地亚语），
     *  反向映射会丢失一些信息
     * ------------------------------------------------------------- */
    export inline const std::map<int, std::string> ID_TO_ISO639_1 = [] {
        std::map<int, std::string> rev;
        for (const auto& p : ISO639_1_TO_ID) {
            rev[p.second] = p.first;  // 后出现的会覆盖前面的
        }
        return rev;
        }();
}
