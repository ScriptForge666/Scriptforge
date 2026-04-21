module;
#include <gtest/gtest.h>
#include <iostream>
#include <filesystem>

export module Local.Test;

import Scriptforge.Pch;
import Scriptforge.Err;
import Scriptforge.LanguageCode;
import Scriptforge.Local;
import Scriptforge.ErrCode;

namespace fs = std::filesystem;
using namespace Scriptforge::LanguageCode;
using namespace Scriptforge::Local;

namespace Scriptforge::Local::Test
{
    TEST(LocalTest, LangDefaultCtorThrows) {
        try {
            Lang{};
            FAIL() << "默认构造应该抛出异常";
        }
        catch (const Scriptforge::Err::BasicError<Scriptforge::ErrCode::ErrCode>& e) {
            EXPECT_EQ(
                e.getCode(),
                Scriptforge::ErrCode::ErrCode::LocalInvalidLanguageCode
            );
            EXPECT_STREQ(
                e.what().c_str(),
                "LanguageIsLegal: Neutral and Invariant are not valid language codes for loading language files."
            );
        }
    }

    TEST(LocalTest, AutoLoadAllLanguages) {
        // 👉 直接返回 vector<Language>
        auto langs = getAvailableLanguages("./lang");
        ASSERT_FALSE(langs.empty()) << "未找到语言文件！";

        for (auto lang : langs) {
            SCOPED_TRACE(("Test language: " +
                ENUM_TO_ISO639_1.at(lang)).c_str());

            Lang loc(lang, "./lang");

            EXPECT_TRUE(loc.isLoaded());
            EXPECT_EQ(loc.getLanguageCode(), lang);
        }
    }
}