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

export module Scriptforge.Msg;

import std;

namespace Scriptforge {
	inline namespace Msg {
		export enum class InformationLevel {
			Debug,
			Info,
			Warning,
			Error,
			Critical
		};

		template <typename T , typename Clock>
		concept MessageRequires = requires(T t1, T t2, Clock c) {
			t1 = t2;
			{ c.now() } -> std::convertible_to<typename Clock::time_point>;
		};

		export 
			template <typename T =std::string, typename Clock = std::chrono::system_clock>
			requires MessageRequires<T, Clock>
		class Message {
		public:
			using TimePoint = typename Clock::time_point;
			Message(const T& msg = T{}, InformationLevel level = InformationLevel::Info, TimePoint tp = Clock.now());
			Message(const T&& msg, InformationLevel level = InformationLevel::Info, TimePoint tp = Clock.now());
			T getMessage() const;
			InformationLevel getLevel() const;
			TimePoint getTime() const;
			friend std::ostream& operator<<(std::ostream& os, const Message<T, Clock>& msg);
		private:
			T m_msg;
			InformationLevel m_level;
			TimePoint m_time;
		};

		using MessageD = Message<>;
	}
}

namespace Scriptforge {
	inline namespace Msg {

		template <typename T, typename Clock>
			requires MessageRequires<T, Clock>
		Message<T, Clock>::Message(const T& msg, InformationLevel level, TimePoint tp) : m_msg(msg), m_level(level), m_time(tp) {}
		
		template <typename T, typename Clock>
			requires MessageRequires<T, Clock>
		Message<T, Clock>::Message(const T&& msg, InformationLevel level, TimePoint tp) : m_msg(msg), m_level(level), m_time(tp) {}
		
		template <typename T, typename Clock>
			requires MessageRequires<T, Clock>
		T Message<T, Clock>::getMessage() const {
			return m_msg;
		}

		template <typename T, typename Clock>
			requires MessageRequires<T, Clock>
		InformationLevel Message<T, Clock>::getLevel() const {
			return m_level;
		}

		template <typename T, typename Clock>
			requires MessageRequires<T, Clock>
		Message<T, Clock>::TimePoint Message<T, Clock>::getTime() const {
			return m_time;
		}

		template <typename T, typename Clock>
			requires MessageRequires<T, Clock>
		std::ostream& operator<<(std::ostream& os, const Message<T, Clock>& msg) {
			os << msg.getMessage();
			return os;
		}
	}
}