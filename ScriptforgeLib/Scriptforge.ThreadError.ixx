// Copyright 2026 Scriptforge
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//     http://www.apache.org/licenses/LICENSE-2.0
// Unless required by applicable law or agreed to in writing, software
// distributed under the License.
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
export module Scriptforge.ThreadError;

import Scriptforge.Local;
import Scriptforge.Err;
import Scriptforge.ErrCode;
import Scriptforge.ErrCode.throwError;
import Scriptforge.Msg;
import Scriptforge.Pch;

namespace Scriptforge {
    inline namespace Err {

        export template <bool Async = false>
            class ThreadError {
            public:
                ThreadError(const Scriptforge::Local::Lang& lang);
                ~ThreadError();

                // 禁止拷贝/移动，线程对象不安全
                ThreadError(const ThreadError&) = delete;
                ThreadError& operator=(const ThreadError&) = delete;
                ThreadError(ThreadError&&) = delete;
                ThreadError& operator=(ThreadError&&) = delete;

                // 任务签名现在可以响应 stop 请求
                void setThreadFunction(std::function<void(std::stop_token)> run);
                void start();
                void stop();

                // 异步模式接口
                void waitForCompletion();
                bool isRunning() const;
                std::future<void> getFuture();

            private:
                // 透传 stop_token；run 使用右值引用避免额外拷贝
                // 使用 shared_ptr promise 以便在构造线程失败时仍能设置异常
                void threadFunc(std::function<void(std::stop_token)>&& run, std::stop_token st, std::shared_ptr<std::promise<void>> prom);
                Scriptforge::Local::Lang m_lang;
                std::jthread m_thread;
                std::future<void> m_completionFuture;
                std::mutex m_mtx;
                std::atomic<bool> m_isRunning{ false };
                std::exception_ptr m_storedException;
                std::function<void(std::stop_token)> m_taskFunc;
        };
    }
}
namespace Scriptforge {
    inline namespace Err {
        template<bool Async>
        ThreadError<Async>::ThreadError(const Scriptforge::Local::Lang& lang) : m_lang(lang) {}

        template <bool Async>
        ThreadError<Async>::~ThreadError() {
            stop();
        }

        template<bool Async>
        void ThreadError<Async>::setThreadFunction(std::function<void(std::stop_token)> run) {
            std::lock_guard<std::mutex> lock(m_mtx);
            m_taskFunc = std::move(run);
        }

        template<bool Async>
        void ThreadError<Async>::start() {
            std::unique_lock<std::mutex> lock(m_mtx);

            if (!m_taskFunc)
                Scriptforge::ErrCode::throwError(Scriptforge::ErrCode::ErrCode::ThreadErrorThreadNoTask, __func__, m_lang);
            if (m_isRunning.load(std::memory_order_acquire))
                Scriptforge::ErrCode::throwError(Scriptforge::ErrCode::ErrCode::ThreadErrorThreadAlreadyRunning, __func__, m_lang);

            m_storedException = nullptr;

            // 将任务移动到局部变量，避免在 lambda 中捕获对成员的引用和额外拷贝
            std::function<void(std::stop_token)> task = std::move(m_taskFunc);

            // 清理可能已结束但仍可 join 的旧线程（在构造新线程前）
            if (!m_isRunning.load(std::memory_order_acquire) && m_thread.joinable()) {
                std::jthread oldThread = std::move(m_thread);
                // 在不持锁的情况下 join
                lock.unlock();
                if (oldThread.joinable()) {
                    try {
                        oldThread.join();
                    }
                    catch (...) {
                        // 忽略 join 中的异常（一般不应抛出），仍需保证状态一致性
                    }
                }
                lock.lock();
            }

            if constexpr (Async) {
                auto promPtr = std::make_shared<std::promise<void>>();
                auto fut = promPtr->get_future();

                // 释放锁再构造线程，避免在构造时持锁导致死锁
                lock.unlock();
                try {
                    std::jthread newThread(
                        [this, task = std::move(task), p = promPtr]
                        (std::stop_token st) mutable
                        {
                            this->threadFunc(std::move(task), st, p);
                        }
                    );

                    // 构造成功后再把状态写回成员（持锁）
                    lock.lock();
                    m_thread = std::move(newThread);
                    m_completionFuture = std::move(fut);
                    m_isRunning.store(true, std::memory_order_release);
                }
                catch (...) {
                    // 在构造线程失败时，确保 promise 有异常可见，以避免 future 进入 broken_promise
                    std::exception_ptr ep = std::current_exception();
                    try {
                        if (promPtr) {
                            promPtr->set_exception(ep);
                        }
                    }
                    catch (...) {
                        // 忽略在设置 promise 异常时可能出现的问题
                    }
                    // 回滚任务，保证对象一致性
                    std::lock_guard<std::mutex> rollbackLock(m_mtx);
                    m_taskFunc = std::move(task);
                    throw;
                }
            }
            else {
                // 非 async 模式同样在构造线程时不持锁
                lock.unlock();
                try {
                    std::jthread newThread(
                        [this, task = std::move(task)] (std::stop_token st) mutable
                        {
                            this->threadFunc(std::move(task), st, std::shared_ptr<std::promise<void>>());
                        }
                    );

                    lock.lock();
                    m_thread = std::move(newThread);
                    m_isRunning.store(true, std::memory_order_release);
                }
                catch (...) {
                    std::lock_guard<std::mutex> rollbackLock(m_mtx);
                    m_taskFunc = std::move(task);
                    throw;
                }
            }
        }

        template<bool Async>
        void ThreadError<Async>::stop() {
            // 快速检查并请求停止（短时间持锁）
            {
                std::lock_guard<std::mutex> lock(m_mtx);
                if (!m_isRunning.load(std::memory_order_acquire)) {
                    return;
                }
                // 如果线程对象可停止则请求停止；request_stop 本身不阻塞
                m_thread.request_stop();
            }

            // 在不持锁的情况下等待线程结束，避免死锁（线程可能在异常处理里加锁）
            if (m_thread.joinable()) {
                m_thread.join();
            }

            // 清理状态
            {
                std::lock_guard<std::mutex> lock(m_mtx);
                m_isRunning.store(false, std::memory_order_release);
                m_thread = std::jthread();
            }
        }

        template<bool Async>
        void ThreadError<Async>::waitForCompletion() {
            if constexpr (Async) {
                // 异步模式通过 future 获取异常并等待完成
                std::unique_lock<std::mutex> lock(m_mtx);
                if (m_completionFuture.valid()) {
                    // 释放锁前把 future 移出本地以免 blocking 时持锁
                    auto fut = std::move(m_completionFuture);
                    lock.unlock();
                    fut.get();
                }
            }
            else {
                // 同步模式：等待线程结束并重抛捕获的异常（如果有）
                // 先等待线程结束（不持锁）
                if (m_thread.joinable()) {
                    m_thread.join();
                }
                // 检查并重抛存储的异常
                std::lock_guard<std::mutex> lock(m_mtx);
                if (m_storedException) {
                    std::rethrow_exception(m_storedException);
                }
            }
        }
        template<bool Async>
        bool ThreadError<Async>::isRunning() const {
            return m_isRunning.load(std::memory_order_acquire);
        }
        template<bool Async>
        std::future<void> ThreadError<Async>::getFuture() {
            if constexpr (Async) {
                std::unique_lock<std::mutex> lock(m_mtx);
                if (!m_completionFuture.valid()) {
                    throw std::logic_error("no valid completion future (it may have been moved)");
                }
                auto fut = std::move(m_completionFuture);
                lock.unlock();
                return fut;
            }
            else {
                throw std::logic_error("getFuture is only available in async mode");
            }
        }
        template<bool Async>
        void ThreadError<Async>::threadFunc(std::function<void(std::stop_token)>&& run, std::stop_token st, std::shared_ptr<std::promise<void>> prom) {
            try {
                // 将 stop_token 透传给业务任务，使其可以中途响应停止
                run(st);
                if (prom) {
                    prom->set_value();
                }
            }
            catch (...) {
                {
                    std::lock_guard<std::mutex> lock(m_mtx);
                    m_storedException = std::current_exception();
                    if (prom) {
                        try {
                            prom->set_exception(m_storedException);
                        }
                        catch (...) {
                            // 忽略 promise 设置异常时可能抛出的错误
                        }
                    }
                }
            }
            // 确保线程结束时清理运行标志
            m_isRunning.store(false, std::memory_order_release);
        }

    }
}