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

//Waring:It is only compatible with Windows!
#if defined(_WIN32) || defined(_WIN64)
export module Scriptforge.AntiDebug;
import <Windows.h>;
import std;

namespace Scriptforge {
	inline namespace AntiDebug {
        export class AntiDebugger {
        public:
            AntiDebugger();
            ~AntiDebugger();
            void start();
            void stop();
            bool is_debugger_present() const;

        private:
            std::atomic<bool> stop_flag;
            std::atomic<bool> debugger;
            std::unique_ptr<std::thread> antiDebugThread;
            std::unique_ptr<std::thread> debugStopperThread;
            std::mutex mtx;
            std::condition_variable cv;
            void anti_debug();
            void debug_stopper();
        };
	}
}

namespace Scriptforge {
    inline namespace AntiDebug {
        AntiDebugger::AntiDebugger() : stop_flag(false), debugger(false), antiDebugThread(nullptr), debugStopperThread(nullptr) {}
        AntiDebugger::~AntiDebugger() {
            stop();
        }
        void AntiDebugger::start() {
            std::thread antiDebug(&AntiDebugger::anti_debug, this);
            std::thread debugStopper(&AntiDebugger::debug_stopper,this);
            antiDebug.detach();
            debugStopper.detach();
        }
        void AntiDebugger::stop() {
            stop_flag.store(true);
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
        bool AntiDebugger::is_debugger_present() const {
            return debugger.load();
        }
        void AntiDebugger::anti_debug() {
            while (!stop_flag.load()) {
               if (IsDebuggerPresent()) {
                    std::lock_guard<std::mutex> lock(mtx);
                    debugger.store(true);
                    stop_flag.store(true);
                    cv.notify_all(); // 触发 debug_stopper 线程
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }
        }
        void AntiDebugger::debug_stopper() {
            while (true) {
                std::unique_lock<std::mutex> lock(mtx);
                cv.wait(lock, [&] { return stop_flag.load() || debugger.load(); });
                if (stop_flag.load() || debugger.load()) {
                std::exit(1);
                }
            }
        }
    }
}
#else
#pragma message("Because Scriptforge.AntiDebug.ixx is only compatible with Windows,it will not be compiled.")
#endif