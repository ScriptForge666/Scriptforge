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
#include "ScriptforgeVersion.hpp"
#include <chrono>
#include <string>
#include <sstream>

namespace Scriptforge::Version {
    std::string getVersion() {
        return std::string(ProjectName) + " version " + std::string(ProjectVersion);
    }

    int getYear() {
        using namespace std::chrono;
        auto now = system_clock::now();
        auto dp = floor<days>(now);
        year_month_day ymd{ dp };
        return int(ymd.year());
    }

    std::string getCopyright() {
        std::ostringstream oss;
        oss << "Copyright " << getYearInterval() << " " << ProjectContributor
            << "\nLicensed under the " << ProjectLicense;
        return oss.str();
    }
    std::string getYearInterval() {
        int currentYear = getYear();
        int startYear = std::stoi(std::string(ProjectStartYear));
        if (currentYear == startYear) {
            return std::string(ProjectStartYear);
        }
        else {
            return std::string(ProjectStartYear) + "~" + std::to_string(currentYear);
        }
    }
}