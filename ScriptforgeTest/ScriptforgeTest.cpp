// Copyright 2025 Scriptforge
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

#include <iostream>
#include <random>
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

int main() {
    Logger logger{ "log.log" };
    cout << versionInfo.getCopyright() << endl;
    logger.log("[main]print\"" + versionInfo.getCopyright() + "\"\n");
    ThreadError t{"t1",logger};
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