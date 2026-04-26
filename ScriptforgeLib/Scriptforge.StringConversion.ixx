module;
#include "utf8/utf8/cpp20.h"
export module Scriptforge.StringConversion;
import Scriptforge.Pch;

namespace Scriptforge::StringConversion {
    export template <typename T>
        concept is_basic_string = requires(T str) {
        requires std::same_as<T, std::basic_string<typename T::value_type>>;
    };

    // ------------------------------
    // 对外唯一接口：str_convert
    // ------------------------------
    export template <class ToStr, class FromStr>
        requires is_basic_string<ToStr>&& is_basic_string<FromStr>
    ToStr str_convert(const FromStr& str);

}

namespace Scriptforge::StringConversion {

    // @brief static_assert
    static_assert(sizeof(char) == sizeof(char8_t), "char and char8_t must be same size");
    static_assert(sizeof(wchar_t) == sizeof(char16_t) || sizeof(wchar_t) == sizeof(char32_t), "wchar_t must be the same size as char16_t or char32_t");

    inline static constexpr bool getWstringSize() {
        return sizeof(wchar_t) == sizeof(char16_t);
    }

    std::u8string stringToU8(const std::string& str) {
        return std::u8string(str.begin(), str.end());
	}
    std::u16string stringToU16(const std::string& str) {
        std::u16string result;
        utf8::utf8to16(str.begin(), str.end(), std::back_inserter(result));
        return result;
	}
    std::u32string stringToU32(const std::string& str) {
        std::u32string result;
        utf8::utf8to32(str.begin(), str.end(), std::back_inserter(result));
        return result;
	}
    std::wstring stringToWstring(const std::string& str) {
        std::wstring result;
        if constexpr (sizeof(wchar_t) == sizeof(char16_t)) {
            std::u16string t;
            t = stringToU16(str);
            result = std::wstring(t.begin(), t.end());
        } else {
            std::u32string t;
            t = stringToU32(str);
            result = std::wstring(t.begin(), t.end());
        }
        return result;
	}
    std::string u8ToString(const std::u8string& str) {
        return std::string(str.begin(), str.end());
    }
    std::string u16ToString(const std::u16string& str) {
        std::string result;
        utf8::utf16to8(str.begin(), str.end(), std::back_inserter(result));
        return result;
    }
    std::string u32ToString(const std::u32string& str) {
        std::string result;
        utf8::utf32to8(str.begin(), str.end(), std::back_inserter(result));
        return result;
    }
    std::string wstringToString(const std::wstring& str) {
        if constexpr (getWstringSize()) {
            return u16ToString(std::u16string(str.begin(), str.end()));
        } else {
            return u32ToString(std::u32string(str.begin(), str.end()));
        }
	}
    std::u8string wstringToU8(const std::wstring& str) {
        if constexpr (getWstringSize()) {
            std::u8string result;
            utf8::utf16to8(str.begin(), str.end(), std::back_inserter(result));
            return result;
        }
        else {
            std::u8string result;
            utf8::utf32to8(str.begin(), str.end(), std::back_inserter(result));
            return result;
        }
    }
    std::u16string wstringToU16(const std::wstring& str) {
        if constexpr (getWstringSize()) {
            return std::u16string(str.begin(), str.end());
        }
        else {
            std::u16string result;
			std::string t = wstringToString(str);
            utf8::utf8to16(t.begin(), t.end(), std::back_inserter(result));
            return result;
        }
	}
    std::u32string wstringToU32(const std::wstring& str) {
        if constexpr (getWstringSize()) {
            std::u32string result;
            std::string t = wstringToString(str);
            utf8::utf8to32(t.begin(), t.end(), std::back_inserter(result));
            return result;
        }
        else {
            return std::u32string(str.begin(), str.end());
        }
	}
}
module;
#include "utf8/utf8/cpp20.h"
export module Scriptforge.StringConversion;
import Scriptforge.Pch;

namespace Scriptforge::StringConversion {
    export template <typename T>
        concept is_basic_string = requires(T str) {
        requires std::same_as<T, std::basic_string<typename T::value_type>>;
    };

    // ------------------------------
    // 对外唯一接口：str_convert
    // ------------------------------
    export template <class ToStr, class FromStr>
        requires is_basic_string<ToStr>&& is_basic_string<FromStr>
    ToStr str_convert(const FromStr& str);

}

namespace Scriptforge::StringConversion {

    // @brief static_assert
    static_assert(sizeof(char) == sizeof(char8_t), "char and char8_t must be same size");
    static_assert(sizeof(wchar_t) == sizeof(char16_t) || sizeof(wchar_t) == sizeof(char32_t), "wchar_t must be the same size as char16_t or char32_t");

    inline static constexpr bool getWstringSize() {
        return sizeof(wchar_t) == sizeof(char16_t);
    }

    std::u8string stringToU8(const std::string& str) {
        return std::u8string(str.begin(), str.end());
	}
    std::u16string stringToU16(const std::string& str) {
        std::u16string result;
        utf8::utf8to16(str.begin(), str.end(), std::back_inserter(result));
        return result;
	}
    std::u32string stringToU32(const std::string& str) {
        std::u32string result;
        utf8::utf8to32(str.begin(), str.end(), std::back_inserter(result));
        return result;
	}
    std::wstring stringToWstring(const std::string& str) {
        std::wstring result;
        if constexpr (sizeof(wchar_t) == sizeof(char16_t)) {
            std::u16string t;
            t = stringToU16(str);
            result = std::wstring(t.begin(), t.end());
        } else {
            std::u32string t;
            t = stringToU32(str);
            result = std::wstring(t.begin(), t.end());
        }
        return result;
	}
    std::string u8ToString(const std::u8string& str) {
        return std::string(str.begin(), str.end());
    }
    std::string u16ToString(const std::u16string& str) {
        std::string result;
        utf8::utf16to8(str.begin(), str.end(), std::back_inserter(result));
        return result;
    }
    std::string u32ToString(const std::u32string& str) {
        std::string result;
        utf8::utf32to8(str.begin(), str.end(), std::back_inserter(result));
        return result;
    }
    std::string wstringToString(const std::wstring& str) {
        if constexpr (getWstringSize()) {
            return u16ToString(std::u16string(str.begin(), str.end()));
        } else {
            return u32ToString(std::u32string(str.begin(), str.end()));
        }
	}
    std::u8string wstringToU8(const std::wstring& str) {
        if constexpr (getWstringSize()) {
            std::u8string result;
            utf8::utf16to8(str.begin(), str.end(), std::back_inserter(result));
            return result;
        }
        else {
            std::u8string result;
            utf8::utf32to8(str.begin(), str.end(), std::back_inserter(result));
            return result;
        }
    }
    std::u16string wstringToU16(const std::wstring& str) {
        if constexpr (getWstringSize()) {
            return std::u16string(str.begin(), str.end());
        }
        else {
            std::u16string result;
			std::string t = wstringToString(str);
            utf8::utf8to16(t.begin(), t.end(), std::back_inserter(result));
            return result;
        }
	}
    std::u32string wstringToU32(const std::wstring& str) {
        if constexpr (getWstringSize()) {
            std::u32string result;
            std::string t = wstringToString(str);
            utf8::utf8to32(t.begin(), t.end(), std::back_inserter(result));
            return result;
        }
        else {
            return std::u32string(str.begin(), str.end());
        }
	}
}
module;
#include "utf8/utf8/cpp20.h"
export module Scriptforge.StringConversion;
import Scriptforge.Pch;

namespace Scriptforge::StringConversion {
    export template <typename T>
        concept is_basic_string = requires(T str) {
        requires std::same_as<T, std::basic_string<typename T::value_type>>;
    };

    // ------------------------------
    // 对外唯一接口：str_convert
    // ------------------------------
    export template <class ToStr, class FromStr>
        requires is_basic_string<ToStr>&& is_basic_string<FromStr>
    ToStr str_convert(const FromStr& str);

}

namespace Scriptforge::StringConversion {

    // @brief static_assert
    static_assert(sizeof(char) == sizeof(char8_t), "char and char8_t must be same size");
    static_assert(sizeof(wchar_t) == sizeof(char16_t) || sizeof(wchar_t) == sizeof(char32_t), "wchar_t must be the same size as char16_t or char32_t");

    inline static constexpr bool getWstringSize() {
        return sizeof(wchar_t) == sizeof(char16_t);
    }

    std::u8string stringToU8(const std::string& str) {
        return std::u8string(str.begin(), str.end());
	}
    std::u16string stringToU16(const std::string& str) {
        std::u16string result;
        utf8::utf8to16(str.begin(), str.end(), std::back_inserter(result));
        return result;
	}
    std::u32string stringToU32(const std::string& str) {
        std::u32string result;
        utf8::utf8to32(str.begin(), str.end(), std::back_inserter(result));
        return result;
	}
    std::wstring stringToWstring(const std::string& str) {
        std::wstring result;
        if constexpr (sizeof(wchar_t) == sizeof(char16_t)) {
            std::u16string t;
            t = stringToU16(str);
            result = std::wstring(t.begin(), t.end());
        } else {
            std::u32string t;
            t = stringToU32(str);
            result = std::wstring(t.begin(), t.end());
        }
        return result;
	}
    std::string u8ToString(const std::u8string& str) {
        return std::string(str.begin(), str.end());
    }
    std::string u16ToString(const std::u16string& str) {
        std::string result;
        utf8::utf16to8(str.begin(), str.end(), std::back_inserter(result));
        return result;
    }
    std::string u32ToString(const std::u32string& str) {
        std::string result;
        utf8::utf32to8(str.begin(), str.end(), std::back_inserter(result));
        return result;
    }
    std::string wstringToString(const std::wstring& str) {
        if constexpr (getWstringSize()) {
            return u16ToString(std::u16string(str.begin(), str.end()));
        } else {
            return u32ToString(std::u32string(str.begin(), str.end()));
        }
	}
    std::u8string wstringToU8(const std::wstring& str) {
        if constexpr (getWstringSize()) {
            std::u8string result;
            utf8::utf16to8(str.begin(), str.end(), std::back_inserter(result));
            return result;
        }
        else {
            std::u8string result;
            utf8::utf32to8(str.begin(), str.end(), std::back_inserter(result));
            return result;
        }
    }
    std::u16string wstringToU16(const std::wstring& str) {
        if constexpr (getWstringSize()) {
            return std::u16string(str.begin(), str.end());
        }
        else {
            std::u16string result;
			std::string t = wstringToString(str);
            utf8::utf8to16(t.begin(), t.end(), std::back_inserter(result));
            return result;
        }
	}
    std::u32string wstringToU32(const std::wstring& str) {
        if constexpr (getWstringSize()) {
            std::u32string result;
            std::string t = wstringToString(str);
            utf8::utf8to32(t.begin(), t.end(), std::back_inserter(result));
            return result;
        }
        else {
            return std::u32string(str.begin(), str.end());
        }
	}
}