
#pragma once
#ifndef SCRIPTFORGE_TIME_HPP
#define SCRIPTFORGE_TIME_HPP

#include <iomanip>
#include <string>

namespace Scriptforge::Time {

    inline std::int64_t unix_epoch_ms();

    inline std::int64_t unix_epoch_us();

    inline std::string format_now_local(int precision = 3); // precision=3 means ms,precision=6 means ns

    inline std::string format_now_seconds();

}
#endif // SCRIPTFORGE_TIME_HPP