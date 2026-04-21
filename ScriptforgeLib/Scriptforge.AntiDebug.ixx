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

//Waring:It is only compatible with Windows!
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
            bool is_debugger_present() const;

        private:
            std::atomic<bool> m_stopFlag;
            std::atomic<bool> m_debugger;
            std::unique_ptr<std::thread> m_antiDebugThread;
            std::mutex m_mtx;
            void antiDebug();
        };
		using AntiDebuger = ADCL;
	}
	namespace AntiDebug = ADNS;
}

namespace Scriptforge {
    inline namespace ADNS {
        ADCL::ADCL() : m_stopFlag(false), m_debugger(false), m_antiDebugThread(nullptr) {}
        ADCL::~ADCL() {
            stop();
        }
        void ADCL::start() {
            std::thread antiDebug(&ADCL::antiDebug, this);
            antiDebug.detach();
        }
        void ADCL::stop() {
            m_stopFlag.store(true);
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
        bool ADCL::is_debugger_present() const {
            return m_debugger.load();
        }
    }
}
#else
#pragma message("Because Scriptforge.AntiDebug.ixx is only compatible with Windows,it will not be compiled.")
#endif