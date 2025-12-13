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
module Scriptforge.Version;

import Scriptforge.Err;
import std;
using namespace Scriptforge::Err;

namespace Scriptforge::BitPack {
	BoolBitPack::BoolBitPack(const classItself& src) : m_bools{ src.m_bools } {}
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
			throw Error{ "BoolBitPack::BoolBitPack: src.size() must be 8." };
		}
		for (size_type i = 0; i < 8; ++i) {
			write(i, src[i]);
		}
	}
	BoolBitPack::classItself& BoolBitPack::operator=(const BoolBitPack::classItself& rhs){}
}