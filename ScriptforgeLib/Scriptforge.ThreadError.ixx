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
import Scriptforge.Msg;
import std;

namespace Scriptforge {
    inline namespace Err {
        export
            template <bool Async = false>
        class ThreadError {
        public:
            ThreadError() = default;
            ~ThreadError();

            template <typename U>
            void threadStart(U run);

            // 异步模式特有方法
            void waitForCompletion();
            bool isRunning() const;
            std::future<void> getFuture();

        private:
            template <typename U>
            void threadFunc(std::exception_ptr& err, U run);

            std::jthread m_thread;
            std::promise<void> m_completionPromise;
            std::atomic<bool> m_isRunning{ false };
            std::exception_ptr m_storedException;
        };

        template <typename T, //it only supports string type for now, but you can change it to any type that supports assignment 
            typename Clock,
            bool Async>
		concept ThreadErrorLRequires = requires(T t1, T t2, Clock c) {
            t1 = t2;
			{ c.now() } -> std::convertible_to<typename Clock::time_point>;
            t1->std::convertible_to<std::string>; // Ensure T can be converted to std::string for logging
		};

        export
			template <typename T = std::string, //it only supports string type for now, but you can change it to any type that supports assignment 
            typename Clock = std::chrono::system_clock,
            bool Async = false>
			requires ThreadErrorLRequires<T, Clock, Async>
        class ThreadErrorL {
        public:
            ThreadErrorL(std::string_view name, Scriptforge::Log::Logger<T, Clock>& logger);
            ThreadErrorL() = delete;
            ~ThreadErrorL();

            template <typename U>
            void threadStart(U run);

            void waitForCompletion();
            bool isRunning() const;
            std::future<void> getFuture();

        private:
            template <typename U>
            void threadFunc(std::exception_ptr& err, U run);

            std::string m_name;
            Scriptforge::Log::Logger<T, Clock>& m_logger;
            std::jthread m_thread;
            std::promise<void> m_completionPromise;
            std::atomic<bool> m_isRunning{ false };
            std::exception_ptr m_storedException;
        };
    }
}

namespace Scriptforge::Err {
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
    template <typename U>
    void ThreadError<Async>::threadFunc(std::exception_ptr& err, U run) {
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
    template <typename U>
    void ThreadError<Async>::threadStart(U run) {
        std::exception_ptr err;

        if constexpr (Async) {
            // 异步模式
            m_completionPromise = std::promise<void>{};

            m_thread = std::jthread([this, run = std::forward<U>(run), &err]() mutable {
                threadFunc(err, std::move(run));
                });

        }
        else {
            // 同步模式
            m_thread = std::jthread([this, run = std::forward<U>(run), &err]() mutable {
                threadFunc(err, std::move(run));
                });

            m_thread.join();

            if (err) {
                std::rethrow_exception(err);
            }
        }
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

    // ThreadErrorL 实现（保持与之前类似）
    template <typename T, typename Clock, bool Async>
        requires ThreadErrorLRequires<T, Clock, Async>
    ThreadErrorL<T, Clock, Async>::ThreadErrorL(std::string_view name,
        Scriptforge::Log::Logger<T, Clock>& logger)
        : m_name(name), m_logger(logger)
    {
        m_logger.log(Scriptforge::Message<T, Clock>{ "[" + m_name + "] Create a new ThreadErrorL(Async = " +
            (Async ? "true" : "false") + ").", Scriptforge::InformationLevel::Info});
    }

    template <typename T, typename Clock, bool Async>
        requires ThreadErrorLRequires<T, Clock, Async>
    ThreadErrorL<T, Clock, Async>::~ThreadErrorL() {
        if constexpr (Async) {
            if (m_isRunning) {
                m_logger.log(Scriptforge::Message<T, Clock>{"[" + m_name + "] Waiting for async task to complete...", Scriptforge::InformationLevel::Info});
                waitForCompletion();
            }
        }
        else {
            if (m_thread.joinable()) {
                m_thread.join();
            }
        }
        m_logger.log(Scriptforge::Message<T, Clock>{"[" + m_name + "] ThreadErrorL destroyed.", Scriptforge::InformationLevel::Info});
    }

    template <typename T, typename Clock, bool Async>
        requires ThreadErrorLRequires<T, Clock, Async>
    template <typename U>
    void ThreadErrorL<T, Clock, Async>::threadFunc(std::exception_ptr& err, U run) {
        m_isRunning = true;
        m_logger.log(Scriptforge::Message<T, Clock>{"[" + m_name + "] Thread started.", Scriptforge::InformationLevel::Info});

        try {
            run();
            if constexpr (Async) {
                m_completionPromise.set_value();
            }
            m_logger.log(Scriptforge::Message<T, Clock>{"[" + m_name + "] Thread completed successfully.", Scriptforge::InformationLevel::Info});
        }
        catch (...) {
            err = std::current_exception();
            m_storedException = err;

            if constexpr (Async) {
                try {
                    std::rethrow_exception(err);
                }
                catch (const std::exception& e) {
                    m_completionPromise.set_exception(std::current_exception());
                }
                catch (...) {
                    m_completionPromise.set_exception(std::make_exception_ptr(
                        std::runtime_error("Unknown exception")));
                }
            }
            m_logger.log(Scriptforge::Message<T, Clock>{"[" + m_name + "] Thread caught exception.", Scriptforge::InformationLevel::Info});
        }

        m_isRunning = false;
    }

    template <typename T, typename Clock, bool Async>
        requires ThreadErrorLRequires<T, Clock, Async>
    template <typename U>
    void ThreadErrorL<T, Clock, Async>::threadStart(U run) {
        std::exception_ptr err;

        if constexpr (Async) {
            m_completionPromise = std::promise<void>{};

            m_thread = std::jthread([this, run = std::forward<U>(run), &err]() mutable {
                threadFunc(err, std::move(run));
                });

            m_logger.log(Scriptforge::Message<T, Clock>{"[" + m_name + "] Async task started.", Scriptforge::InformationLevel::Info});

        }
        else {
            m_thread = std::jthread([this, run = std::forward<U>(run), &err]() mutable {
                threadFunc(err, std::move(run));
                });

            m_thread.join();

            if (err) {
                m_logger.log(Scriptforge::Message<T, Clock>{"[" + m_name + "] Synchronous task failed.", Scriptforge::InformationLevel::Info});
                std::rethrow_exception(err);
            }
        }
    }

    template <typename T, typename Clock, bool Async>
        requires ThreadErrorLRequires<T, Clock, Async>
    void ThreadErrorL<T, Clock, Async>::waitForCompletion() {
        static_assert(Async, "waitForCompletion() is only available in async mode");

        if (m_isRunning) {
            m_logger.log(Scriptforge::Message<T, Clock>{"[" + m_name + "] Waiting for completion...", Scriptforge::InformationLevel::Info});
            getFuture().wait();

            if (m_storedException) {
                m_logger.log(Scriptforge::Message<T, Clock>{"[" + m_name + "] Re-throwing stored exception.", Scriptforge::InformationLevel::Info});
                std::rethrow_exception(m_storedException);
            }
            m_logger.log(Scriptforge::Message<T, Clock>{"[" + m_name + "] Async task completed.", Scriptforge::InformationLevel::Info});
        }
    }

    template <typename T, typename Clock, bool Async>
        requires ThreadErrorLRequires<T, Clock, Async>
    bool ThreadErrorL<T, Clock, Async>::isRunning() const {
        return m_isRunning;
    }

    template <typename T, typename Clock, bool Async>
        requires ThreadErrorLRequires<T, Clock, Async>
    std::future<void> ThreadErrorL<T, Clock, Async>::getFuture() {
        static_assert(Async, "getFuture() is only available in async mode");
        return m_completionPromise.get_future();
    }
}