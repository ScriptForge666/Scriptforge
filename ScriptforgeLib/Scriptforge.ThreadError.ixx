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

import Scriptforge.Local;
import Scriptforge.Err;
import Scriptforge.Msg;
import Scriptforge.Pch;

namespace Scriptforge {
    inline namespace Err {
        export
            template <bool Async = false>
        class ThreadError {
        public:
            ThreadError() = default;
            ~ThreadError();

            void threadStart(std::function<void()> run);
            void setThreadFunction(std::function<void()> run);
            void start();

            // 异步模式特有方法
            void waitForCompletion();
            bool isRunning() const;
            std::future<void> getFuture();

        private:
            void threadFunc(std::exception_ptr& err, std::function<void()> run);

            std::jthread m_thread;
            std::promise<void> m_completionPromise;
            std::atomic<bool> m_isRunning{ false };
            std::exception_ptr m_storedException;
			std::function<void()> m_taskFunc;
        };
    }
}

namespace Scriptforge {
    inline namespace Err {
        // ThreadError 实现
        template <bool Async>
        ThreadError<Async>::~ThreadError() {
            if constexpr (Async) {
                if (m_isRunning) {
                    waitForCompletion();
                }
            }
            else {
                if (m_thread.joinable()) {
                    m_thread.join();
                }
            }
        }

        template <bool Async>
        void ThreadError<Async>::threadFunc(std::exception_ptr& err, std::function<void()> run) {
            m_isRunning = true;

            try {
                run();
                if constexpr (Async) {
                    m_completionPromise.set_value();
                }
            }
            catch (...) {
                err = std::current_exception();
                m_storedException = err;  // 保存异常供后续使用

                if constexpr (Async) {
                    try {
                        std::rethrow_exception(err);
                    }
                    catch (...) {
                        m_completionPromise.set_exception(std::current_exception());
                    }
                }
            }

            m_isRunning = false;
        }

        template <bool Async>
        void ThreadError<Async>::setThreadFunction(std::function<void()> run) {
            m_taskFunc = std::move(run);
        }

        template <bool Async>
        void ThreadError<Async>::threadStart(std::function<void()> run) {
            std::exception_ptr err;
			m_taskFunc = std::move(run);
            if constexpr (Async) {
                // 异步模式
                m_completionPromise = std::promise<void>{};

                m_thread = std::jthread([this, run = m_taskFunc, &err]() mutable {
                    threadFunc(err, std::move(run));
                    });

            }
            else {
                // 同步模式
                m_thread = std::jthread([this, run = m_taskFunc, &err]() mutable {
                    threadFunc(err, std::move(run));
                    });

                m_thread.join();

                if (err) {
                    std::rethrow_exception(err);
                }
            }
        }

        template <bool Async>
        void ThreadError<Async>::start() {
            threadStart(m_taskFunc);
        }

        template <bool Async>
        void ThreadError<Async>::waitForCompletion() {
            static_assert(Async, "waitForCompletion() is only available in async mode");

            if (m_isRunning) {
                getFuture().wait();

                // 如果有异常，重新抛出
                if (m_storedException) {
                    std::rethrow_exception(m_storedException);
                }
            }
        }

        template <bool Async>
        bool ThreadError<Async>::isRunning() const {
            return m_isRunning;
        }

        template <bool Async>
        std::future<void> ThreadError<Async>::getFuture() {
            static_assert(Async, "getFuture() is only available in async mode");
            return m_completionPromise.get_future();
        }
    }
}