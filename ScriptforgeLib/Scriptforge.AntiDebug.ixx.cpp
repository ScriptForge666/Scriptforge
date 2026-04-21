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
 * @brief 定义了反调试功能模块
 * @author Scriptforge
 * @date 2026/3/29
 */

 // Warning: It is only compatible with Windows!
#if defined(_WIN32) || defined(_WIN64)

#define RAND_PASTE(a, b) a##b
#define RAND_NAME(x) RAND_PASTE(x, __COUNTER__)

export module Scriptforge.AntiDebug;
import <Windows.h>;
import Scriptforge.Pch;

namespace Scriptforge::AntiDebug
{
    namespace RAND_NAME(Impl_)
    {
        export class RAND_NAME(ADbg_)
        {
        public:
            ADbg_();
            ~ADbg_();
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
    }

    using AntiDebugger = RAND_NAME(Impl_)::RAND_NAME(ADbg_);
}

namespace Scriptforge::AntiDebug
{
    using Impl = RAND_NAME(Impl_);
    using ADbg = Impl::RAND_NAME(ADbg_);

    ADbg::ADbg_() : m_stopFlag(false), m_debugger(false), m_antiDebugThread(nullptr) {}
    ADbg::~ADbg_() { stop(); }

    void ADbg::start()
    {
        std::thread t(&ADbg::antiDebug, this);
        t.detach();
    }

    void ADbg::stop()
    {
        m_stopFlag = true;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    bool ADbg::is_debugger_present() const
    {
        return m_debugger;
    }
}

#else
#pragma message("Scriptforge::AntiDebug is Windows-only, skipped.")
#endif