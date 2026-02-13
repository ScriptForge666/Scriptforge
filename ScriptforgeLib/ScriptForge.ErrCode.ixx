export module ScriptForge.ErrCode;
import std;
import Scriptforge.Err;

namespace Scriptforge::ErrCode {
	export enum class ErrCode {
		Tree0001 = 10001,      // Tree0001: Invalid node
		Tree0002 = 10002,      // Tree0002: Empty node
		Tree0003 = 10003,      // Tree0003: Orphaned node
		Log0001 = 20002,       // Log0001: Cannot open log file
		BitPack0001 = 30001, //BitPack0001: Invalid size for packing
		BitPack0002 = 30002, //BitPack0002: Value too large to pack
	};
	std::string toString(ErrCode code) {
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
		default:
			return "Unknown error";
		}
		result += std::to_string(static_cast<int>(code) % 10000);
		return result;
	}
}