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

import std;

namespace Scriptforge::Version {
    VersionInfo::VersionInfo(std::string_view ProjectName, std::string_view ProjectVersion,
        std::string_view ProjectContributor, std::string_view ProjectLicense,
        std::string_view ProjectStartYear)
        : m_ProjectName(ProjectName),
        m_ProjectVersion(ProjectVersion),
        m_ProjectContributor(ProjectContributor),
        m_ProjectLicense(ProjectLicense),
        m_ProjectStartYear(ProjectStartYear),
        m_ProjectStopYear(""),
        m_alreadyStopped(false)
    {
    }
    
    std::string VersionInfo::getVersion() const {
        return std::string(m_ProjectName) + " version " + std::string(m_ProjectVersion);
    }

    int VersionInfo::getYear() const {
        using namespace std::chrono;
        auto now = system_clock::now();
        auto dp = floor<days>(now);
        year_month_day ymd{ dp };
        return int(ymd.year());
    }

    std::string VersionInfo::getCopyright() const {
        std::ostringstream oss;
        oss << "Copyright " << getYearInterval() << " " << m_ProjectContributor
            << "\nLicensed under the " << m_ProjectLicense;
        return oss.str();
    }
    std::string VersionInfo::getYearInterval() const {
        int currentYear = getYear();
        int startYear = std::stoi(std::string(m_ProjectStartYear));
        if (currentYear == startYear) {
            return std::string(m_ProjectStartYear);
        }
        else {
            return std::string(m_ProjectStartYear) + "~" + std::to_string(currentYear);
        }
    }
}