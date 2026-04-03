module;
#include<gtest/gtest.h>
export module LocalTest;

import Scriptforge.LanguageCode;
import Scriptforge.Local;

namespace Scriptforge::Local::Test {
    TEST(LocalTest, GetLanguageName) {
        using namespace Scriptforge::LanguageCode;
        using namespace Scriptforge::Local;

        // ✅ 修复：第二个参数传正确路径！
        Lang lang{ Language::Chinese, "./lang" };

        std::string languageName = lang.getLanguageName<std::string>();
        EXPECT_EQ(languageName, "简体中文");
    }
}