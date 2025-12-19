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

namespace Scriptforge::BitPack {
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