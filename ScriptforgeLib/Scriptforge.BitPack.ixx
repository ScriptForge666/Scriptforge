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

/**
 * @file Scriptforge.BitPack.ixx
 * @brief 定义了 `Scriptforge::BitPack` 模块，其中包含了一个 `BoolBitPack` 类，用于将八个布尔值打包成一个字节。该类提供了构造函数、赋值运算符、比较运算符、索引运算符以及读写方法，可以方便地操作和访问打包的布尔值。此外，还提供了一个方法将打包的布尔值转换为字节类型。
 * @author Scriptforge
 * @date 2026/3/29
 */

export module Scriptforge.BitPack;

import Scriptforge.Err;
import Scriptforge.ErrCode;
import Scriptforge.ErrCode.throwError;
import Scriptforge.Local;
import Scriptforge.Pch;

export namespace Scriptforge {
	inline namespace BitPack {
		export class BoolBitPack {
		public:
			using value_type = bool;
			using size_type = size_t;
			using classItself = BoolBitPack;
			BoolBitPack() = default;
			BoolBitPack(const BoolBitPack& src) = default;
			BoolBitPack(BoolBitPack&&) = default;
			explicit BoolBitPack(const value_type bool1,
				const value_type bool2,
				const value_type bool3,
				const value_type bool4,
				const value_type bool5,
				const value_type bool6,
				const value_type bool7,
				const value_type bool8,
				Scriptforge::Lang lang = Scriptforge::Lang{});
			explicit BoolBitPack(const std::span<bool> src, Scriptforge::Lang lang = Scriptforge::Lang{});
			classItself& operator=(const BoolBitPack& rhs);
			classItself& operator=(const std::span<bool> rhs);
			bool operator==(const BoolBitPack& rhs) const = default;
			bool operator==(const std::span<bool> rhs) const;
			bool operator!=(const BoolBitPack& rhs) const = default;
			bool operator!=(const std::span<bool> rhs) const;
			value_type operator[](size_type x);
			const value_type operator[](size_type x) const;
			void write(const size_type& where, const value_type what);
			void write(const size_type&& where, const value_type what);
			value_type read(const size_type& where) const;
			value_type read(const size_type&& where) const;
			void change(const size_type& where);
			void change(const size_type&& where);
			void clear(const value_type what = false);
			std::byte toByte() const;
		private:
			inline void set_bit(std::byte& b, size_type pos, value_type val);
			std::byte m_bools{ std::byte{ 0 } };
			Scriptforge::Lang m_lang;
		};
	}
}

namespace Scriptforge {
	inline namespace BitPack {
		BoolBitPack::BoolBitPack(const value_type bool1,
			const value_type bool2,
			const value_type bool3,
			const value_type bool4,
			const value_type bool5,
			const value_type bool6,
			const value_type bool7,
			const value_type bool8,
			Scriptforge::Lang lang) {
			write(0, bool1);
			write(1, bool2);
			write(2, bool3);
			write(3, bool4);
			write(4, bool5);
			write(5, bool6);
			write(6, bool7);
			write(7, bool8);
			m_lang = lang;
		}
		BoolBitPack::BoolBitPack(const std::span<bool> src, Scriptforge::Lang lang) 
			: m_lang(lang) {
			if (src.size() != 8) {
				Scriptforge::ErrCode::throwError(Scriptforge::ErrCode::ErrCode::BitPackInvalidSizeForPacking, __func__, m_lang);
			}
			for (size_type i = 0; i < 8; ++i) {
				write(i, src[i]);
			}
		}
		BoolBitPack& BoolBitPack::operator=(const BoolBitPack& rhs) {
			if (this == &rhs)
				return *this;
			m_bools = rhs.toByte();
			return *this;
		}
		BoolBitPack& BoolBitPack::operator=(const std::span<bool> rhs) {
			if (rhs.size() != 8) {
				Scriptforge::ErrCode::throwError(Scriptforge::ErrCode::ErrCode::BitPackInvalidSizeForPacking, __func__, m_lang);
			}
			for (size_type i = 0; i < 8; ++i) {
				write(i, rhs[i]);
			}
			return *this;
		}
		bool BoolBitPack::operator==(const std::span<bool> rhs) const {
			if (rhs.size() != 8) {
				Scriptforge::ErrCode::throwError(Scriptforge::ErrCode::ErrCode::BitPackInvalidSizeForPacking, __func__, m_lang);
			}
			for (size_type i = 0; i < 8; ++i) {
				if (read(i) != rhs[i]) {
					return false;
				}
			}
			return true;
		}
		bool BoolBitPack::operator!=(const std::span<bool> rhs) const {
			return !(*this == rhs);
		}
		BoolBitPack::value_type BoolBitPack::operator[](size_type x) {
			return read(x);
		}
		const BoolBitPack::value_type BoolBitPack::operator[](size_type x) const {
			return read(x);
		}

		void BoolBitPack::write(const size_type& where, const value_type what) {
			if (where >= 8)
				Scriptforge::ErrCode::throwError(Scriptforge::ErrCode::ErrCode::BitPackValueTooLargeToPack, __func__, m_lang);
			set_bit(m_bools, where, what);
		}

		void BoolBitPack::write(const size_type&& where, const value_type what) {
			write(where, what);
		}

		BoolBitPack::value_type BoolBitPack::read(const size_type& where) const {
			if (where >= 8)
				Scriptforge::ErrCode::throwError(Scriptforge::ErrCode::ErrCode::BitPackValueTooLargeToPack, __func__, m_lang);
			auto bits = std::to_integer<unsigned char>(m_bools);
			return static_cast<value_type>(bits & (1 << where));
		}
		BoolBitPack::value_type BoolBitPack::read(const size_type&& where) const {
			return read(where);
		}

		void BoolBitPack::change(const size_type& where) {
			write(where, !read(where));
		}

		void BoolBitPack::change(const size_type&& where) {
			change(where);
		}

		void BoolBitPack::clear(const value_type what) {
			for (size_type i = 0; i < 8; ++i) {
				write(i, what);
			}
		}

		std::byte BoolBitPack::toByte() const {
			return m_bools;
		}

		inline void BoolBitPack::set_bit(std::byte& b, size_type pos, value_type val) {
			auto bits = std::to_integer<unsigned char>(b);
			if (val)
				bits |= (1 << pos);
			else
				bits &= ~(1 << pos);
			b = std::byte(bits);
		}
	}
}