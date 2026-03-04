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

		export 
			template <typename T =std::string, typename Clock = std::chrono::system_clock>
			requires requires(T t1, T t2, Clock c) {
			t1 = t2;
			{ c.now() } -> std::convertible_to<typename Clock::time_point>;
		}
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

		template <typename T = std::string, typename Clock = std::chrono::system_clock>
			requires requires(T t1, T t2, Clock c) {
			t1 = t2;
			{ c.now() } -> std::convertible_to<typename Clock::time_point>;
		}
		Message<T, Clock>::Message(const T& msg, InformationLevel level, TimePoint tp) : m_msg(msg), m_level(level), m_time(tp) {}
		
		template <typename T = std::string, typename Clock = std::chrono::system_clock>
			requires requires(T t1, T t2, Clock c) {
			t1 = t2;
			{ c.now() } -> std::convertible_to<typename Clock::time_point>;
		}
		Message<T, Clock>::Message(const T&& msg, InformationLevel level, TimePoint tp) : m_msg(msg), m_level(level), m_time(tp) {}
		
		template <typename T = std::string, typename Clock = std::chrono::system_clock>
			requires requires(T t1, T t2, Clock c) {
			t1 = t2;
			{ c.now() } -> std::convertible_to<typename Clock::time_point>;
		}
		T Message<T, Clock>::getMessage() const {
			return m_msg;
		}

		template <typename T = std::string, typename Clock = std::chrono::system_clock>
			requires requires(T t1, T t2, Clock c) {
			t1 = t2;
			{ c.now() } -> std::convertible_to<typename Clock::time_point>;
		}
		InformationLevel Message<T, Clock>::getLevel() const {
			return m_level;
		}

		template <typename T = std::string, typename Clock = std::chrono::system_clock>
			requires requires(T t1, T t2, Clock c) {
			t1 = t2;
			{ c.now() } -> std::convertible_to<typename Clock::time_point>;
		}
		Message<T, Clock>::TimePoint Message<T, Clock>::getTime() const {
			return m_time;
		}

		template <typename T = std::string, typename Clock = std::chrono::system_clock>
			requires requires(T t1, T t2, Clock c) {
			t1 = t2;
			{ c.now() } -> std::convertible_to<typename Clock::time_point>;
		}
		std::ostream& operator<<(std::ostream& os, const Message<T, Clock>& msg) {
			os << msg.getMessage();
			return os;
		}
	}
}