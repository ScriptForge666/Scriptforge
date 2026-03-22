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
module;
namespace Scriptforge {
	inline namespace Local {
		class Lang;
	}
}
export module Scriptforge.ErrCode;
import std;
import Scriptforge.Err;

namespace Scriptforge::ErrCode {
	export enum class ErrCode {
		TreeInvalidNode = 10001,      // Tree0001: Invalid node
		TreeEmptyNode = 10002,      // Tree0002: Empty node
		TreeOrphanedNode = 10003,      // Tree0003: Orphaned node
		LogCannotOpenLogFile = 20001,       // Log0001: Cannot open log file
		BitPackInvalidSizeForPacking = 30001, //BitPack0001: Invalid size for packing
		BitPackValueTooLargeToPack = 30002, //BitPack0002: Value too large to pack
		LocalLanguageFileNotFound = 40001,    // Local0001: Language file not found
		LocalInvalidLanguageCode = 40002,     // Local0002: Invalid language code
		RingBufferCapacityBeZero = 50001        // RingBuffer0001: Capacity must be greater than 0
	};
	export std::string toString(ErrCode code) {
		std::string result;
		int codeType = static_cast<int>(code) / 10000;
		switch (codeType) {
		case 0:
			result += "Err";
			break;
		case 1:
			result += "Tree";
			break;
		case 2:
			result += "Log";
			break;
		case 3:
			result += "BitPack";
			break;
		case 4:
			result += "Local";
			break;
		case 5:
			result += "RingBuffer";
			break;
		default:
			return "Unknown error";
		}
		result += std::to_string(static_cast<int>(code) % 10000);
		return result;
	}
	export [[noreturn]] void throwError(
		ErrCode code,
		const std::string& func,
		const Scriptforge::Local::Lang& lang,
		const std::vector <std::string> args = {}
	);
}
