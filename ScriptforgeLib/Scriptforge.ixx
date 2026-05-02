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
 * @file Scriptforge.ixx
 * @brief 定义了 `Scriptforge` 模块，并导入了该模块下的多个子模块，包括 `AntiDebug`、`Err`、`Log`、`Tree` 和 `Version`。这些子模块分别提供了反调试功能、错误处理功能、日志记录功能、树结构操作功能和版本信息功能。通过使用这些子模块，可以方便地实现各种功能，提高代码的可读性和可维护性。
 * @author Scriptforge
 * @date 2026/3/29
 */

export module Scriptforge;

export import Scriptforge.AntiDebug;
export import Scriptforge.Err;
export import Scriptforge.Log;
export import Scriptforge.Tree;
export import Scriptforge.Version;

import std;
constexpr std::string_view ProjectName{ "Scriptforge" };
constexpr std::string_view ProjectVersion{ "0.1.2-dev-withBug" };
constexpr std::string_view ProjectContributor{ "Scriptforge" };
constexpr std::string_view ProjectLicense{ "Apache License 2.0"};
constexpr std::chrono::year ProjectStartYear{ 2025 };
namespace Scriptforge {
		inline namespace Version {
			export Scriptforge::Version::VersionInfo versionInfo{
				ProjectName,
				ProjectVersion,
				ProjectContributor,
				ProjectLicense,
				ProjectStartYear
			};

	}

}
