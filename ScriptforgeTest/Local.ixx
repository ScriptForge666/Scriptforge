module;
#include<gtest/gtest.h>
export module LocalTest;

import Scriptforge.Err;
import Scriptforge.LanguageCode;
import Scriptforge.Local;

namespace Scriptforge::Local::Test {
    TEST(LocalTest, LangWithoutArg) {
        using namespace Scriptforge::LanguageCode;
        using namespace Scriptforge::Local;
        try {
            Lang{};
        }
        catch (const Scriptforge::Err::Error& e) {
            EXPECT_EQ(e, "Lang: Neutral and Invariant are not valid language codes for loading language files.")
        }
    }
}