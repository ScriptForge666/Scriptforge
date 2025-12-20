// Copyright 2025 Scriptforge
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//     http://www.apache.org/licenses/LICENSE-2.0
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
export module Scriptforge.BitPack;

import std;

export namespace Scriptforge::BitPack {
	export class BoolBitPack {
	public:
		using value_type = bool;
		using size_type = std::size_t;
		using classItself = BoolBitPack;
		BoolBitPack() = default;
		BoolBitPack(const classItself& src) = default;
		BoolBitPack(classItself&&) = default;
		explicit BoolBitPack(const value_type bool1,
			const value_type bool2,
			const value_type bool3,
			const value_type bool4,
			const value_type bool5,
			const value_type bool6,
			const value_type bool7,
			const value_type bool8);
		explicit BoolBitPack(const std::span<bool> src);
		classItself& operator=(const classItself& rhs);
		classItself& operator=(const std::span<bool> rhs);
		bool operator==(const classItself& rhs) const = default;
		bool operator==(const std::span<bool> rhs) const;
		bool operator!=(const classItself& rhs) const = default;
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
	};
}

module :private;
import Scriptforge.Err;

namespace Scriptforge::BitPack {
	BoolBitPack::BoolBitPack(const value_type bool1,
		const value_type bool2,
		const value_type bool3,
		const value_type bool4,
		const value_type bool5,
		const value_type bool6,
		const value_type bool7,
		const value_type bool8) {
		write(0, bool1);
		write(1, bool2);
		write(2, bool3);
		write(3, bool4);
		write(4, bool5);
		write(5, bool6);
		write(6, bool7);
		write(7, bool8);
	}
	BoolBitPack::BoolBitPack(const std::span<bool> src) {
		if (src.size() != 8) {
			throw Scriptforge::Err::Error{ "BoolBit001",std::string(__func__)+":size must be 8." };
		}
		for (size_type i = 0; i < 8; ++i) {
			write(i, src[i]);
		}
	}
	BoolBitPack::classItself& BoolBitPack::operator=(const BoolBitPack::classItself& rhs) {
		if (this == &rhs)
			return *this;
		m_bools = rhs.toByte();
		return *this;
	}
	BoolBitPack::classItself& BoolBitPack::operator=(const std::span<bool> rhs) {
		if (rhs.size() != 8) {
			throw Scriptforge::Err::Error{ "BoolBit001",std::string(__func__) + ":size must be 8." };
		}
		for (size_type i = 0; i < 8; ++i) {
			write(i, rhs[i]);
		}
		return *this;
	}
	bool BoolBitPack::operator==(const std::span<bool> rhs) const {
		if (rhs.size() != 8) {
			throw Scriptforge::Err::Error{ "BoolBit001",std::string(__func__) + ":size must be 8." };
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
			throw Scriptforge::Err::Error{ "BoolBit002",std::string(__func__) + ":where must be less than 8." };
		set_bit(m_bools, where, what);
	}

	void BoolBitPack::write(const size_type&& where, const value_type what) {
		write(where, what);
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