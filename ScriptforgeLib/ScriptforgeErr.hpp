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
#ifndef SCRIPTFORGE_ERR_HPP
#define SCRIPTFORGE_ERR_HPP
import std;

namespace Scriptforge::Err {
    //建议用E0100~E9999,E0001~E0099留给库内容
    class Error {
    public:
        Error() = default;
        Error(std::string_view error);
        Error(std::string_view code, std::string_view error);

        std::string_view what() const;
        std::string_view code() const;

        friend std::ostream& operator<<(std::ostream& os, const Error& err);

    private:
        std::string m_code{ "No Code" };
        std::string m_error{ "Unknown Error" };
    };
}
#endif // !SCRIPTFORGE_ERR_HPP