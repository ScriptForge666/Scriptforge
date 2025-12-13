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
#pragma once
#ifndef SCRIPTFORGE_VERSION_HPP
#define SCRIPTFORGE_VERSION_HPP
#include <string>
#include <string_view>

namespace Scriptforge::Version {
	inline constexpr std::string_view ProjectName = "Scriptforge";
	inline constexpr std::string_view ProjectVersion = "0.9.14";
	inline constexpr std::string_view ProjectContributor = "Scriptforge";
	inline constexpr std::string_view ProjectLicense = "Apache License 2.0";
	inline constexpr std::string_view ProjectStartYear = "2025";

	std::string getVersion();
	std::string getCopyright();
	int getYear();
	std::string getYearInterval();
}
#endif// !SCRIPTFORGE_VERSION_HPP