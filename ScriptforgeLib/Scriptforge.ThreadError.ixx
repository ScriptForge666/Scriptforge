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

                void setThreadFunction(std::function<void()> run);
                void start();
				void stop();

                // 异步模式接口
                void waitForCompletion();
                bool isRunning() const;
                std::future<void> getFuture();

            private:
                void threadFunc(std::function<void()> run, std::optional<std::promise<void>> prom);
				Scriptforge::Local::Lang m_lang;
                std::jthread m_thread;
                std::future<void> m_completionFuture;
                std::mutex m_mtx;
                std::atomic<bool> m_isRunning{ false };
                std::exception_ptr m_storedException;
                std::function<void()> m_taskFunc;
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
        void ThreadError<Async>::setThreadFunction(std::function<void()> run) {
            std::lock_guard<std::mutex> lock(m_mtx);
            m_taskFunc = std::move(run);
		}

		template<bool Async>
        void ThreadError<Async>::start() {

            std::lock_guard<std::mutex> lock(m_mtx);

            if(!m_taskFunc)
				Scriptforge::ErrCode::throwError(Scriptforge::ErrCode::ErrCode::ThreadErrorThreadNoTask, __func__, m_lang);
            if (m_isRunning)
				Scriptforge::ErrCode::throwError(Scriptforge::ErrCode::ErrCode::ThreadErrorThreadAlreadyRunning, __func__, m_lang);

            m_storedException = nullptr;
            m_isRunning = true;
            if constexpr (Async) {
				std::promise<void> prom;
				m_completionFuture = prom.get_future();
                m_thread = std::jthread(
                    [this, task = m_taskFunc, pOpt = std::move(promOpt)]
                    (std::stop_token /* jthread 自动传入 */) mutable
                    {
                        this->threadFunc(std::move(task), std::move(pOpt));
                    }
                );
            }
            else {
                m_thread = std::jthread(std::bind(&ThreadError<Async>::threadFunc, this, m_taskFunc, std::nullopt));
            }
		}
    }
}