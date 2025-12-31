// Copyright 2025-2026 Scriptforge
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//     http://www.apache.org/licenses/LICENSE-2.0
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
import Scriptforge;

#include <time.h>
import std;
using namespace std;
using namespace Scriptforge;

int threadcheck(int i) {
    if (i % 2 == 0) {
        return i;
    }
    else {
        throw Error{ "E01","It's a singular!" };
    }
}
//Made By GPT-oss:120b
inline double random_normal(double mean = 0.0, double stddev = 1.0)
{
    thread_local std::mt19937 engine([]() -> std::mt19937 {
        std::random_device rd;
        std::seed_seq seq{
            rd(), rd(), rd(),
            static_cast<unsigned>(std::chrono::high_resolution_clock::now().
                                 time_since_epoch().count())
        };
        return std::mt19937{ seq };
        }());
    std::normal_distribution<double> dist(mean, stddev);

    return dist(engine);
}

//Made By Qwen3:8b and Myself
bool is2026NewYearsDay() {
    auto now = std::chrono::system_clock::now();
    time_t now_c = std::chrono::system_clock::to_time_t(now);
    std::tm local_tm;
    errno_t err = 0;

#ifdef _MSC_VER
    err = localtime_s(&local_tm, &now_c);
#else
    std::tm* local_tm_ptr = std::localtime(&now_c);
    if (!local_tm_ptr) {
        return false;
    }
    local_tm = *local_tm_ptr;
#endif

    if (err != 0) {
        return false;
    }

    int year = local_tm.tm_year + 1900;
    int month = local_tm.tm_mon + 1;
    int day = local_tm.tm_mday;
    return year == 2026 && month == 1 && day <= 5 && day >= 1;
}


int main() {
#if defined(_WIN32) || defined(_WIN64)
    AntiDebugger debug;
    debug.start();
#else
#pragma message("Because Scriptforge.AntiDebug.ixx is only compatible with Windows,it will not compile.")
#endif
    Logger logger{ "log.log" };
    if (is2026NewYearsDay()) {
        cout << "Happy New Year!🎉(A surprise)\n";
        logger.log("[main]print\"Happy New Year!🎉(A surprise)\"\n");
    }
    cout << versionInfo.getCopyright() << endl;
    logger.log("[main]print\"" + versionInfo.getCopyright() + "\"\n");
    ThreadErrorL t{"t1",logger};
    double random = random_normal(1.0, 200.0);
    logger.log("[main]log random number\"" + to_string(random) + "\"\n");
    auto tlamada = [random]() {threadcheck(static_cast<int>(random)); };
    try {
        t.threadStart(tlamada);
    }
    catch (const Error& e) {
        logger.log("[main]caught exception:" + string{ e.what() });
        cout << e<<endl;
    }
     return 0;
 }