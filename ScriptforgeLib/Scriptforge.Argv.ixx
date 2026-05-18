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

/**
 * @file Scriptforge.Argv.ixx
 * @brief 定义了 `Scriptforge::Argv` 模块，其中包含了一个 `Argv` 类，用于处理命令行参数。
 * @author Scriptforge
 * @date 2026/5/25
 */

module;
#include <cstdint>
export module Scriptforge.Argv;

import Scriptforge.Pch;

namespace Scriptforge::Argv {
    namespace internal {
        export inline std::vector<std::string> args;
    }

    export void init(int argc, char* argv[]);
    export std::optional<std::string> get(std::string_view key);

    /**
     * @details 一个用于检查类型是否为有效的 argv 命令的概念。
     * 可以编写以下类型的命令结构体/类：
     * ```cpp
     * struct ArgvCommand {
     *     static constexpr std::string_view name = "command";
     *     static constexpr std::string_view shortName = "c";
     *     static void run() {
     *         std::cout << "Running command!" << std::endl;
     *     }
     * };
     * ```
    */
    template<typename T>
    concept isArgvCommand = requires {
        { T::name } -> std::convertible_to<std::string_view>;
        { T::shortName } -> std::convertible_to<std::string_view>;
            requires requires { T::run(); };
    };

    /**
    * @details 一个用于检查类型是否为有效的 argv 未知命令的概念。
    * 可以编写以下类型的命令结构体/类：
    * ```cpp
    * struct ArgvUnknown {
    *     static void run() {
    *         std::cout << "Running unknown command!" << std::endl;
    *     }
    * };
    * ```
   */
    template<typename T>
    concept isArgvUnknown = requires { T::run(); };

    export
        template<isArgvUnknown UnknownCommand, isArgvCommand... Commands>
    void run();

    constexpr uint32_t hash(std::string_view s);

    template<isArgvUnknown UnknownCommand, isArgvCommand... Commands>
    struct CommandJumpTable {
        static void run(const std::vector<std::string>& args);
    };
}

namespace Scriptforge::Argv {
    void init(int argc, char* argv[]) {
        internal::args.assign(argv + 1, argv + argc);
    }

    export std::optional<std::string> get(std::string_view key) {
        auto it = std::ranges::find(internal::args, key);

        // 找不到 key 或已经是最后一项 → 返回空
        if (it == internal::args.end() || std::next(it) == internal::args.end())
            return {};

        // 核心判断：下一项 必须 不带 "-"，才是值
        // 带 "-" 就是下一个选项/命令，直接返回空！
        const auto& next_arg = *std::next(it);
        if (!next_arg.starts_with('-')) {
            return next_arg; // 是值，返回
        }

        // 下一项是选项/命令，不读取
        return std::nullopt;
    }

    template<isArgvUnknown UnknownCommand, isArgvCommand... Commands>
    void run() {
        CommandJumpTable<UnknownCommand, Commands...>::run(internal::args);
    }

    constexpr uint32_t hash(std::string_view s) {
        uint32_t h = 0x811C9DC5u;
        for (char c : s)
            h = (h ^ (uint8_t)c) * 0x01000193u;
        return h;
    }

    template<isArgvUnknown UnknownCommand, isArgvCommand... Commands>
    void CommandJumpTable<UnknownCommand, Commands...>::run(const std::vector<std::string>& args) {
        for (const auto& arg : args) {
            uint32_t h = hash(arg);

            bool found = false;

            // 折叠表达式 = 生成 SWITCH 跳转表
            ([&] {
                if (h == hash(Commands::name) ||
                    h == hash(Commands::shortName)) {
                    Commands::run();
                    found = true;
                }
                }(), ...);
            if (!found) {
                UnknownCommand::run();
            }
        }
    }

}