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

/**
 * @file Scriptforge.Msg.ixx
 * @brief 定义了 `Scriptforge::Msg` 模块，其中包含了一个 `Message` 类，用于表示日志消息。该类包含消息内容、信息级别和时间戳等成员变量，并提供了相应的构造函数和成员函数来获取这些信息。此外，还定义了一个友元函数用于将消息输出到流中。通过使用该类，可以方便地创建和处理日志消息，提高代码的可读性和可维护性。
 * @author Scriptforge
 * @date 2026/3/29
 */

module;

#include <ctime>

export module Scriptforge.Msg;

import Scriptforge.Pch;

namespace Scriptforge {
	inline namespace Msg {
		export enum class InformationLevel {
			Debug,
			Info,
			Warning,
			Error,
			Critical
		};

		export std::string getInformationLevel(InformationLevel level) {
			switch (level) {
			case InformationLevel::Debug:
				return "Debug";
			case InformationLevel::Info:
				return "Info";
			case InformationLevel::Warning:
				return "Warning";
			case InformationLevel::Error:
				return "Error";
			case InformationLevel::Critical:
				return "Critical";
			default:
				return "Unknown";
			}
		}

		export
			template <typename T, typename Clock>
		concept MessageRequires = 
			std::copyable<T> &&
			std::copyable<Clock> &&
			requires {
				typename Clock::time_point;
				{ Clock::now() } -> std::convertible_to<typename Clock::time_point>;
				//{ Clock::to_time_t(std::declval<typename Clock::time_point>()) } -> std::convertible_to<typename time_t>;
			};

		export 
			template <typename T =std::string, typename Clock = std::chrono::system_clock>
			requires MessageRequires<T, Clock>
		class BasicMessage {
		public:
			using TimePoint = typename Clock::time_point;
			BasicMessage(const T& msg = T{}, InformationLevel level = InformationLevel::Info, TimePoint tp = Clock::now());
			BasicMessage(T&& msg, InformationLevel level = InformationLevel::Info, TimePoint tp = Clock::now());
			const T& message() const noexcept;
			InformationLevel level() const noexcept;
			TimePoint time() const noexcept;
		private:
			T m_msg;
			InformationLevel m_level;
			TimePoint m_time;
		};

		export 
			template <typename T, typename Clock>
			requires MessageRequires<T, Clock>&&
			requires { Clock::to_time_t(std::declval<typename Clock::time_point>()); }&&
			requires { !std::is_same_v<Clock, std::chrono::steady_clock>; }
		std::ostream& operator<<(std::ostream& os, const BasicMessage<T, Clock>& msg);

		export 
			template <typename T, typename Clock>
			requires MessageRequires<T, Clock>&&
		std::same_as<Clock, std::chrono::steady_clock>
			std::ostream& operator<<(std::ostream& os, const BasicMessage<T, Clock>& msg);

		export using Message = BasicMessage<>;
	}
}

namespace Scriptforge {
	inline namespace Msg {

		template <typename T, typename Clock>
			requires MessageRequires<T, Clock>
		BasicMessage<T, Clock>::BasicMessage(const T& msg, InformationLevel level, TimePoint tp) : m_msg(msg), m_level(level), m_time(tp) {}

		template <typename T, typename Clock>
			requires MessageRequires<T, Clock>
		BasicMessage<T, Clock>::BasicMessage(T&& msg, InformationLevel level, TimePoint tp) : m_msg(std::move(msg)), m_level(level), m_time(tp) {}

		template <typename T, typename Clock>
			requires MessageRequires<T, Clock>
		const T& BasicMessage<T, Clock>::message() const noexcept {
			return m_msg;
		}

		template <typename T, typename Clock>
			requires MessageRequires<T, Clock>
		InformationLevel BasicMessage<T, Clock>::level() const noexcept {
			return m_level;
		}

		template <typename T, typename Clock>
			requires MessageRequires<T, Clock>
		typename BasicMessage<T, Clock>::TimePoint BasicMessage<T, Clock>::time() const noexcept {
			return m_time;
		}

		template <typename T, typename Clock>
			requires MessageRequires<T, Clock> &&
			requires { Clock::to_time_t(std::declval<typename Clock::time_point>()); } &&
			requires { !std::is_same_v<Clock, std::chrono::steady_clock>; }
		std::ostream& operator<<(std::ostream& os, const BasicMessage<T, Clock>& msg) {
			auto time_t = Clock::to_time_t(msg.time());
			os << "[" << std::put_time(localtime(&time_t), "%Y-%m-%d %H:%M:%S")
				<< " | " << getInformationLevel(msg.level())
				<< "] " << msg.message();

			return os;
		}

		template <typename T, typename Clock>
			requires MessageRequires<T, Clock>&&
		std::same_as<Clock, std::chrono::steady_clock>
			std::ostream& operator<<(std::ostream& os, const BasicMessage<T, Clock>& msg) {
			os << "[ " << msg.time().time_since_epoch().count()
				<< " | " << getInformationLevel(msg.level())
				<< "] " << msg.message();
			return os;
		}
	}
}