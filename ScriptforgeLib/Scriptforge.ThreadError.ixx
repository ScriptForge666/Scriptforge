// Copyright 2026 Scriptforge
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//     http://www.apache.org/licenses/LICENSE-2.0
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
export module Scriptforge.ThreadError;

import Scriptforge.Log;
import Scriptforge.Err;
import std;

namespace Scriptforge {
	inline namespace Err {
        export class ThreadError {
        public:
            ThreadError() = default;
            template <typename T>
            void threadStart(T run);
        private:
            template <typename T>
            void threadFunc(std::exception_ptr& err, T run);
        };

        export class ThreadErrorL {
        public:
            ThreadErrorL(std::string_view name, Scriptforge::Log::Logger& logger);
            ThreadErrorL() = delete;
            template <typename T>
            void threadStart(T run);
        private:
            template <typename T>
            void threadFunc(std::exception_ptr& err, T run);
            std::string_view m_name;
            Scriptforge::Log::Logger& m_logger;
        };
	}
}

namespace Scriptforge {
    inline namespace Err {
        template <typename T>
        void ThreadError::threadFunc(std::exception_ptr& err, T run) {
            try {
                run();
            }
            catch (...) {
                err = std::current_exception();
            }
        }
        template <typename T>
        void ThreadError::threadStart(T run) {
            std::exception_ptr err;
            std::thread t(&ThreadErrorL::threadFunc<T>, this,
                std::ref(err), std::forward<decltype(run)>(run));
            t.join();
            if (err) {
                std::rethrow_exception(err);
            }
        }

        ThreadErrorL::ThreadErrorL(std::string_view name, Scriptforge::Log::Logger& logger) :m_name(name), m_logger(logger) { m_logger.log("[" + static_cast<std::string>(m_name) + "]" + "Create a new TreadError."); }

        template <typename T>
        void ThreadErrorL::threadFunc(std::exception_ptr& err, T run) {
            try {
                run();
            }
            catch (...) {
                err = std::current_exception();
                m_logger.log("[" + static_cast<std::string>(m_name) + "]" + "Thread caught exception.");
            }
        }
        template <typename T>
        void ThreadErrorL::threadStart(T run) {
            std::exception_ptr err;
            std::thread t(&ThreadErrorL::threadFunc<T>, this,
                std::ref(err), std::forward<decltype(run)>(run));
            t.join();
            if (err) {
                m_logger.log("[" + static_cast<std::string>(m_name) + "]" + "Main thread caught exception.");
                std::rethrow_exception(err);
            }
        }
    }
}