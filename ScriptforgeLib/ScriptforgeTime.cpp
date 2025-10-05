#include "ScriptforgeTime.hpp"
#include <chrono>
#include <ctime>
#include <sstream>
#include <iomanip>
#include <string>

namespace Scriptforge::Time {
    inline std::int64_t unix_epoch_ms() {
        using namespace std::chrono;
        return duration_cast<milliseconds>(
            system_clock::now().time_since_epoch()).count();
    }

    inline std::int64_t unix_epoch_us() {
        using namespace std::chrono;
        return duration_cast<microseconds>(
            system_clock::now().time_since_epoch()).count();
    }

    inline std::string format_now_local(int precision) {
        using namespace std::chrono;
        auto now = system_clock::now();
        auto tt = system_clock::to_time_t(now);

        std::tm tm = {};
#if defined(_WIN32)
        localtime_s(&tm, &tt);
#else
        localtime_r(&tt, &tm);
#endif

        std::ostringstream oss;
        oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");

        if (precision >= 3) {
            auto ms = duration_cast<milliseconds>(now.time_since_epoch()) % 1000;
            oss << '.' << std::setfill('0') << std::setw(3) << ms.count();
        }
        if (precision >= 6) {
            auto us = duration_cast<microseconds>(now.time_since_epoch()) % 1000;
            oss << std::setfill('0') << std::setw(3) << us.count();
        }
        return oss.str();
    }

    inline std::string format_now_seconds() {
        return format_now_local(0);
    }
}