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
module Scriptforge.ErrCode;
import Scriptforge.ErrCode;
import Scriptforge.Local;
import Scriptforge.Err;
import std;


namespace Scriptforge::ErrCode {
    void throwTreeError(
        ErrCode code,
        const std::string& func,
        const Scriptforge::Local::Lang& lang,
        const std::vector <std::string> args
    ) {

        Scriptforge::Local::Lang default_en_lang{ std::locale{"en-US"}, lang.getLangPath() };

        std::string default_en_msg = default_en_lang.get(std::to_string(static_cast<int>(code)), toString(code));

        std::string base_msg;
        
		if (args.empty()) {
            base_msg = lang.get(std::to_string(static_cast<int>(code)), default_en_msg);
        } else {
            base_msg = lang.format(std::to_string(static_cast<int>(code)), args);
            if (base_msg == std::to_string(static_cast<int>(code))) {
                base_msg = default_en_lang.format(std::to_string(static_cast<int>(code)), args);
            }
        }

        std::string full_msg = func + ":" + base_msg;

        throw Scriptforge::Error{ toString(code), full_msg };
    }
}