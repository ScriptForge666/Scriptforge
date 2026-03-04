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

export module Scriptforge.Log;
import Scriptforge.Err;
import Scriptforge.ErrCode;
import Scriptforge.Msg;

import std;

namespace Scriptforge {
    inline namespace Log {
        namespace fs = std::filesystem;

        export
            template <typename T = std::string, typename Clock = std::chrono::system_clock>
            requires requires(T t1, T t2, Clock c) {
            t1 = t2;
            { c.now() } -> std::convertible_to<typename Clock::time_point>;
        }
        class Logger {
        public:
            Logger(const std::string& filename = "log.log", const Scriptforge::InformationLevel level = Scriptforge::InformationLevel::Info);
            Logger(const std::string&& filename, const Scriptforge::InformationLevel level = Scriptforge::InformationLevel::Info);
            Logger(const fs::path& file, const Scriptforge::InformationLevel level = Scriptforge::InformationLevel::Info);
            Logger(const Logger& logger) = delete;
            Logger& operator=(const Logger& logger) = delete;
            ~Logger();

            void setLogLevel(const Scriptforge::InformationLevel level);
            Scriptforge::InformationLevel getLogLevel() const;

            void log(const Scriptforge::Message<T, Clock>& msg);
            std::string getFilename() const;
            fs::path getPath() const;

        private:
            void process();
            std::queue<Scriptforge::Message<T, Clock>> logQueue;
            std::mutex mtx;
            std::condition_variable cv;
            std::atomic<bool> running{ true };
            std::thread logThread;
            std::ofstream logFile;
            fs::path m_file;
            Scriptforge::InformationLevel m_logLevel;
        };

        // 实现部分
        template <typename T, typename Clock>
            requires requires(T t1, T t2, Clock c) {
            t1 = t2;
            { c.now() } -> std::convertible_to<typename Clock::time_point>;
        }
        Logger<T, Clock>::Logger(const std::string& filename, const Scriptforge::InformationLevel level)
            : m_logLevel(level) {
            m_file = fs::path(filename);
            logFile.open(m_file, std::ios::app);
            if (!logFile.is_open()) {
                throw Scriptforge::Err::Error{
                    Scriptforge::ErrCode::toString(Scriptforge::ErrCode::ErrCode::LogCannotOpenLogFile),
                    "Cannot open log file: " + filename
                };
            }
            logThread = std::thread(&Logger<T, Clock>::process, this);
        }

        template <typename T, typename Clock>
            requires requires(T t1, T t2, Clock c) {
            t1 = t2;
            { c.now() } -> std::convertible_to<typename Clock::time_point>;
        }
        Logger<T, Clock>::Logger(const std::string&& filename, const Scriptforge::InformationLevel level)
            : Logger(std::string(filename), level) {
        }

        template <typename T, typename Clock>
            requires requires(T t1, T t2, Clock c) {
            t1 = t2;
            { c.now() } -> std::convertible_to<typename Clock::time_point>;
        }
        Logger<T, Clock>::Logger(const fs::path& file, const Scriptforge::InformationLevel level)
            : m_logLevel(level) {
            m_file = file;
            logFile.open(m_file, std::ios::app);
            if (!logFile.is_open()) {
                throw Scriptforge::Err::Error{
                    Scriptforge::ErrCode::toString(Scriptforge::ErrCode::ErrCode::LogCannotOpenLogFile),
                    "Cannot open log file: " + m_file.string()
                };
            }
            logThread = std::thread(&Logger<T, Clock>::process, this);
        }

        template <typename T, typename Clock>
            requires requires(T t1, T t2, Clock c) {
            t1 = t2;
            { c.now() } -> std::convertible_to<typename Clock::time_point>;
        }
        Logger<T, Clock>::~Logger() {
            running = false;
            cv.notify_all();
            if (logThread.joinable()) {
                logThread.join();
            }
            if (logFile.is_open()) {
                logFile.close();
            }
        }

        template <typename T, typename Clock>
            requires requires(T t1, T t2, Clock c) {
            t1 = t2;
            { c.now() } -> std::convertible_to<typename Clock::time_point>;
        }
        void Logger<T, Clock>::process() {
            while (running) {
                std::unique_lock<std::mutex> lock(mtx);
                cv.wait(lock, [this] { return !logQueue.empty() || !running; });

                while (!logQueue.empty()) {
                    auto msg = logQueue.front();
                    logFile << msg.getMessage() << std::endl;
                    logQueue.pop();
                }

                logFile.flush();
            }

            // 处理剩余的消息
            std::lock_guard<std::mutex> lock(mtx);
            while (!logQueue.empty()) {
                auto msg = logQueue.front();
                logFile << msg.getMessage() << std::endl;
                logQueue.pop();
            }
            logFile.flush();
        }

        template <typename T, typename Clock>
            requires requires(T t1, T t2, Clock c) {
            t1 = t2;
            { c.now() } -> std::convertible_to<typename Clock::time_point>;
        }
        void Logger<T, Clock>::setLogLevel(const Scriptforge::InformationLevel level) {
            std::lock_guard<std::mutex> lock(mtx);
            m_logLevel = level;
        }

        template <typename T, typename Clock>
            requires requires(T t1, T t2, Clock c) {
            t1 = t2;
            { c.now() } -> std::convertible_to<typename Clock::time_point>;
        }
        Scriptforge::InformationLevel Logger<T, Clock>::getLogLevel() const {
            return m_logLevel;
        }

        template <typename T, typename Clock>
            requires requires(T t1, T t2, Clock c) {
            t1 = t2;
            { c.now() } -> std::convertible_to<typename Clock::time_point>;
        }
        void Logger<T, Clock>::log(const Scriptforge::Message<T, Clock>& msg) {
            std::lock_guard<std::mutex> lock(mtx);
            if (static_cast<int>(msg.getLevel()) >= static_cast<int>(m_logLevel)) {
                logQueue.push(msg);
                cv.notify_one();
            }
        }

        template <typename T, typename Clock>
            requires requires(T t1, T t2, Clock c) {
            t1 = t2;
            { c.now() } -> std::convertible_to<typename Clock::time_point>;
        }
        std::string Logger<T, Clock>::getFilename() const {
            return m_file.string();
        }

        template <typename T, typename Clock>
            requires requires(T t1, T t2, Clock c) {
            t1 = t2;
            { c.now() } -> std::convertible_to<typename Clock::time_point>;
        }
        fs::path Logger<T, Clock>::getPath() const {
            return m_file;
        }
    }
}
