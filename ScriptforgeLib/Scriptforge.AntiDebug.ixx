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

/**
 * @file Scriptforge.AntiDebug.ixx
 * @brief 定义了 `Scriptforge::AntiDebug` 模块，其中包含了一个 `AntiDebugger` 类，用于实现反调试功能。该类提供了启动和停止反调试线程的方法，以及一个检查是否存在调试器的方法。在反调试线程中，程序会定期检查是否有调试器附加，如果检测到调试器，则会执行相应的反调试操作，如输出调试信息、终止进程等。
 * @author Scriptforge
 * @date 2026/3/29
 */

// Warning: Only compatible with Windows!
module;
#include "random.h"

#if defined(_WIN32) || defined(_WIN64)

export module Scriptforge.AntiDebug;
import <Windows.h>;
import Scriptforge.Pch;

namespace Scriptforge {
    inline namespace ADNS {
        export class ADCL {
        public:
            ADCL();
            ~ADCL();

            void start();
            void stop();
            bool isDebuggerPresent() const;

        private:
            bool isAntiDebug() noexcept;
            void killProcess() noexcept;
            void antiDebug();

            std::atomic<bool> m_stopFlag;
            std::atomic<bool> m_debugger;
            std::mutex m_mtx;
        };

        export using AntiDebugger = ADCL;
    }

    namespace AntiDebug = ADNS;
}

#else
#pragma message("Warning: Scriptforge.AntiDebug is Windows-only. Skipped compilation.")
export module Scriptforge.AntiDebug;
import Scriptforge.Pch;
namespace Scriptforge::AntiDebug { export using AntiDebugger = void; }
#endif