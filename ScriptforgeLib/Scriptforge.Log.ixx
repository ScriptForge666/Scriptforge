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
import	Scriptforge.Err;
import Scriptforge.ErrCode;

import std;

namespace Scriptforge {
	inline namespace Log {
		namespace fs = std::filesystem;

		export enum class InformationLevel {
			Debug,
			Info,
			Warning,
			Error,
			Critical
		};

		export class Msg {
		public:
			Msg(const std::string& msg, InformationLevel level = InformationLevel::Info);
			Msg(const std::string&& msg, InformationLevel level = InformationLevel::Info);
			std::string getMessage() const;
			InformationLevel getLevel() const;
			friend std::ostream& operator<<(std::ostream& os, const Msg& msg);
		private:
			std::string m_msg;
			InformationLevel m_level;
		};

		export class Logger {
		public:
			Logger(const std::string& filename = "log.log", const InformationLevel level = InformationLevel::Info);
			Logger(const std::string&& filename, const InformationLevel level = InformationLevel::Info);
			Logger(const fs::path& file, const InformationLevel level = InformationLevel::Info);
			Logger(const Logger& logger) = delete;
			~Logger();
			void setLogLevel(const InformationLevel level);
			InformationLevel getLogLevel() const;
			void log(const Msg& msg);
			std::string getFilename() const;
			fs::path getPath() const;
		private:
			void process();
			std::queue<Msg> logQueue;
			std::mutex mtx;
			std::condition_variable cv;
			std::atomic<bool> running{ true };
			std::thread logThread;
			std::ofstream logFile;
			fs::path m_file;
			InformationLevel m_logLevel;
		};
	}
} 

namespace Scriptforge {
	inline namespace Log {

		Msg::Msg(const std::string& msg, InformationLevel level) : m_msg(msg), m_level(level) {}

		Msg::Msg(const std::string&& msg, InformationLevel level) : m_msg(msg), m_level(level) {}

		std::string Msg::getMessage() const {
			return m_msg;
		}

		InformationLevel Msg::getLevel() const {
			return m_level;
		}

		std::ostream& operator<<(std::ostream& os, const Msg& msg) {
			os << msg.getMessage();
			return os;
		}

		void Logger::process() {
			while (running || !logQueue.empty()) {
				std::unique_lock<std::mutex> lock(mtx);
				cv.wait(lock, [&] { return !logQueue.empty() || !running; });
				while (!logQueue.empty()) {
					logFile << logQueue.front() << std::endl;
					logQueue.pop();
				}
			}
		}

		Logger::Logger(const std::string& filename, const InformationLevel level)
			: m_logLevel(level) {
			m_file = fs::path(filename);
			logFile.open(m_file);
			if (!logFile.is_open()) {
				throw Scriptforge::Err::Error{ Scriptforge::ErrCode::toString(Scriptforge::ErrCode::ErrCode::LogCannotOpenLogFile), "Cannot open log file: " + filename };
			}
			logThread = std::thread(&Logger::process, this);
		}


		Logger::Logger(const std::string&& filename, const InformationLevel level)
			: m_logLevel(level) {
			m_file = fs::path(filename);
			logFile.open(m_file);
			if (!logFile.is_open()) {
				throw Scriptforge::Err::Error{ Scriptforge::ErrCode::toString(Scriptforge::ErrCode::ErrCode::LogCannotOpenLogFile), "Cannot open log file: " + filename };
			}
			logThread = std::thread(&Logger::process, this);
		}


		Logger::Logger(const fs::path& file, const InformationLevel level) : logFile(file), m_logLevel(level) {
			m_file = static_cast<fs::path>(file);
			logThread = std::thread(&Logger::process, this);
		}

		void Logger::setLogLevel(const InformationLevel level) {
			m_logLevel = level;
		}

		InformationLevel Logger::getLogLevel() const {
			return m_logLevel;
		}

		Logger::~Logger() {
			running = false;
			cv.notify_all();
			logThread.join();
			logFile.close();
		}

		void Logger::log(const Msg& msg) {
			std::lock_guard<std::mutex> lock(mtx);
			if (static_cast<int>(msg.getLevel()) >= static_cast<int>(m_logLevel)) {
				logQueue.push(msg.getMessage());
				cv.notify_one();
			}
		}

		std::string Logger::getFilename() const {
			return m_file.string();
		}

		fs::path Logger::getPath() const {
			return m_file;
		}
	}
}
