module;

#include<gtest/gtest.h>

export module StringConversion.Test;

import Scriptforge.Pch;
import Scriptforge.StringConversion;
using namespace Scriptforge::StringConversion;

namespace Scriptforge::StringConversion::Test {
    TEST(StringConversionTest, ConvertSameType_ReturnsOriginal) {
        // 同类型转换，直接返回原字符串，不做处理
        std::string s = "test";
        EXPECT_EQ(str_convert<std::string>(s), s);

        std::u8string u8 = u8"test";
        EXPECT_EQ(str_convert<std::u8string>(u8), u8);

        std::u16string u16 = u"test";
        EXPECT_EQ(str_convert<std::u16string>(u16), u16);

        std::u32string u32 = U"test";
        EXPECT_EQ(str_convert<std::u32string>(u32), u32);

        std::wstring w = L"test";
        EXPECT_EQ(str_convert<std::wstring>(w), w);
    }

    TEST(StringConversionTest, ConvertStringAndU8String) {
        // std::string ↔ std::u8string（底层字节强转）
        const char* str = "hello utf8";
        const char8_t* u8str = u8"hello utf8";

        std::string s(str);
        auto to_u8 = str_convert<std::u8string>(s);
        EXPECT_EQ(memcmp(to_u8.data(), u8str, to_u8.size()), 0);

        std::u8string u8(u8str);
        auto to_str = str_convert<std::string>(u8);
        EXPECT_EQ(to_str, s);
    }

    TEST(StringConversionTest, ConvertU8AndU16) {
        // u8 ↔ u16
        std::u8string u8 = u8"你好世界 🌍";
        std::u16string u16 = u"你好世界 🌍";

        auto conv_u16 = str_convert<std::u16string>(u8);
        EXPECT_EQ(conv_u16, u16);

        auto conv_u8 = str_convert<std::u8string>(u16);
        EXPECT_EQ(conv_u8, u8);
    }

    TEST(StringConversionTest, ConvertU8AndU32) {
        // u8 ↔ u32
        std::u8string u8 = u8"hello 世界 🌐";
        std::u32string u32 = U"hello 世界 🌐";

        auto conv_u32 = str_convert<std::u32string>(u8);
        EXPECT_EQ(conv_u32, u32);

        auto conv_u8 = str_convert<std::u8string>(u32);
        EXPECT_EQ(conv_u8, u8);
    }

    TEST(StringConversionTest, ConvertU8AndWString) {
        // u8 ↔ wstring（自动适配 Windows / Linux）
        std::u8string u8 = u8"测试字符串 😊";
        std::wstring w;

        if constexpr (sizeof(wchar_t) == 2) {
            w = L"测试字符串 😊";
        }
        else {
            w = L"测试字符串 😊";
        }

        auto to_w = str_convert<std::wstring>(u8);
        EXPECT_EQ(to_w, w);

        auto to_u8 = str_convert<std::u8string>(w);
        EXPECT_EQ(to_u8, u8);
    }

    TEST(StringConversionTest, ConvertStringToAnyType_ViaU8) {
        // string → 任意类型（自动中转 u8）
        std::string str = "abc 123 中文";

        auto to_u8 = str_convert<std::u8string>(str);
        auto to_u16 = str_convert<std::u16string>(str);
        auto to_u32 = str_convert<std::u32string>(str);
        auto to_w = str_convert<std::wstring>(str);

        EXPECT_EQ(str_convert<std::string>(to_u8), str);
        EXPECT_EQ(str_convert<std::string>(to_u16), str);
        EXPECT_EQ(str_convert<std::string>(to_u32), str);
        EXPECT_EQ(str_convert<std::string>(to_w), str);
    }

    TEST(StringConversionTest, ConvertAnyTypeToString_ViaU8) {
        // 任意类型 → string（自动中转 u8）
        std::u8string u8 = u8"test 测试";
        std::u16string u16 = u"test 测试";
        std::u32string u32 = U"test 测试";
        std::wstring w = L"test 测试";

        std::string target = "test 测试";

        EXPECT_EQ(str_convert<std::string>(u8), target);
        EXPECT_EQ(str_convert<std::string>(u16), target);
        EXPECT_EQ(str_convert<std::string>(u32), target);
        EXPECT_EQ(str_convert<std::string>(w), target);
    }

    TEST(StringConversionTest, EmptyStringConversion) {
        // 空字符串边界测试
        std::string empty_str;
        std::u8string empty_u8;
        std::u16string empty_u16;
        std::u32string empty_u32;
        std::wstring empty_w;

        EXPECT_EQ(str_convert<std::u8string>(empty_str), empty_u8);
        EXPECT_EQ(str_convert<std::string>(empty_u8), empty_str);
        EXPECT_EQ(str_convert<std::wstring>(empty_u8), empty_w);
        EXPECT_EQ(str_convert<std::u16string>(empty_w), empty_u16);
    }
}