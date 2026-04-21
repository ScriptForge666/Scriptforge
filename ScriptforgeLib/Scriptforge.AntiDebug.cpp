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
 * @file Scriptforge.AntiDebug.cpp
 * @brief 由于MSVC不支持将内联汇编放在模块接口单元中，因此将 `Scriptforge::AntiDebug` 模块中的`Scriptforge::AntiDebug::AntiDebugger::anti_debug`函数实现放在了这个源文件中。
 * @author Scriptforge
 * @date 2026/4/25
*/

// Warning: It is only compatible with Windows!
module;
#if defined(_WIN32) || defined(_WIN64)

// 全局头文件必须放在这里！
#include <Windows.h>
#include <tlhelp32.h>
#include <intrin.h>
#include <winternl.h>
#include <psapi.h>
#include <cstdint>
#include "random.h"
module Scriptforge.AntiDebug;
import Scriptforge.Pch;

namespace Scriptforge {
    inline namespace ADNS {
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable: 4702)
#endif

        namespace { // 匿名命名空间，符号不导出
            // 编译期字符串哈希
            constexpr uint32_t hashConst(const char* str, uint32_t value = 0) {
                return *str ? hashConst(str + 1, (value << 5) - value + *str) : value;
            }
            HMODULE getNtdllByHash() {
                HMODULE hMods[1024] = { 0 };
                DWORD cbNeeded = 0;

                if (EnumProcessModules(GetCurrentProcess(), hMods, sizeof(hMods), &cbNeeded)) {
                    for (DWORD i = 0; i < (cbNeeded / sizeof(HMODULE)); i++) {
                        char szModName[MAX_PATH] = { 0 };
                        GetModuleBaseNameA(GetCurrentProcess(), hMods[i], szModName, MAX_PATH);

                        // hashConst("ntdll.dll") = 0x53A1A181
                        if (hashConst(szModName) == 0x53A1A181u) {
                            return hMods[i];
                        }
                    }
                }
                return GetModuleHandleA("ntdll.dll");
            }

            /**
             * @brief 获取PEB(Process Environment Block)地址
             * @return PEB的内存地址，获取失败返回0
             */
            UINT_PTR getPeb() {
#ifdef _MSC_VER
#ifdef _WIN64
                return __readgsqword(0x60);
#else
                return __readfsdword(0x30);
#endif
#else
                HMODULE ntdll = GetModuleHandleA("ntdll.dll");
                if (!ntdll) return 0;
                using NtQIP_t = NTSTATUS(NTAPI*)(HANDLE, UINT, PVOID, ULONG, PULONG);
                auto ntQIP = reinterpret_cast<NtQIP_t>(GetProcAddress(ntdll, "NtQueryInformationProcess"));
                if (!ntQIP) return 0;
                ULONG64 out = 0;
                if (NT_SUCCESS(ntQIP(GetCurrentProcess(), 0, &out, sizeof(out), 0))) {
                    return static_cast<UINT_PTR>(out);
                }
                return 0;
#endif
            }

            /**
             * @brief 强制终止当前进程（多重保障，无法跳过）
             */
            void killProcess() {
                HMODULE ntdll = getNtdllByHash();
                if (!ntdll) return;
                using NtTerminateProcess_t = NTSTATUS(NTAPI*)(HANDLE, NTSTATUS);
                auto ntTerm = reinterpret_cast<NtTerminateProcess_t>(GetProcAddress(ntdll, "NtTerminateProcess"));
                if (ntTerm) {
                    ntTerm(GetCurrentProcess(), 0);
                }
                TerminateProcess(GetCurrentProcess(), 0);
                ExitProcess(0);
                abort();
            }

            /**
             * @brief 检查进程是否被附加调试器（通过调试端口）
             * @return 存在调试器返回true，否则返回false
             */
            bool checkDebugPort() {
                HMODULE ntdll = GetModuleHandleA("ntdll.dll");
                if (!ntdll) return false;
                using NtQIP_t = NTSTATUS(NTAPI*)(HANDLE, UINT, PVOID, ULONG, PULONG);
                auto ntQIP = reinterpret_cast<NtQIP_t>(GetProcAddress(ntdll, "NtQueryInformationProcess"));
                if (!ntQIP) return false;
                ULONG64 debugPort = 0;
                NTSTATUS status = ntQIP(GetCurrentProcess(), 7, &debugPort, sizeof(debugPort), 0);
                return NT_SUCCESS(status) && debugPort != 0;
            }

            /**
             * @brief 检查调试寄存器是否被设置
             * @return 调试寄存器有值返回true，否则返回false
             */
            bool checkDr() {
#ifdef _WIN64
                return __readdr(0) || __readdr(1) || __readdr(2) || __readdr(3);
#else
                CONTEXT ctx{};
                ctx.ContextFlags = CONTEXT_DEBUG_REGISTERS;
                if (GetThreadContext(GetCurrentThread(), &ctx)) {
                    return ctx.Dr0 || ctx.Dr1 || ctx.Dr2 || ctx.Dr3;
                }
                return false;
#endif
            }

            /**
             * @brief 检查父进程是否为调试器进程
             * @return 父进程是调试器返回true，否则返回false
             */
            bool checkParentDebug() {
                DWORD processId = GetCurrentProcessId();
                DWORD parentProcessId = 0;
                HANDLE snapshotHandle = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
                if (snapshotHandle == INVALID_HANDLE_VALUE) {
                    return false;
                }

                PROCESSENTRY32 processEntry{ sizeof(processEntry) };
                if (Process32First(snapshotHandle, &processEntry)) {
                    do {
                        if (processEntry.th32ProcessID == processId) {
                            parentProcessId = processEntry.th32ParentProcessID;
                            break;
                        }
                    } while (Process32Next(snapshotHandle, &processEntry));
                }
                CloseHandle(snapshotHandle);
                if (parentProcessId == 0) {
                    return false;
                }

                wchar_t processName[MAX_PATH]{};
                HANDLE parentProcessHandle = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, 0, parentProcessId);
                if (parentProcessHandle) {
                    DWORD nameSize = MAX_PATH;
                    if (QueryFullProcessImageNameW(parentProcessHandle, 0, processName, &nameSize)) {
                        wchar_t* fileName = wcsrchr(processName, L'\\');
                        if (fileName) {
                            const bool isDebugger = !_wcsicmp(fileName + 1, L"x64dbg.exe") ||
                                !_wcsicmp(fileName + 1, L"x32dbg.exe") ||
                                !_wcsicmp(fileName + 1, L"ollydbg.exe") ||
                                !_wcsicmp(fileName + 1, L"devenv.exe");
                            CloseHandle(parentProcessHandle);
                            return isDebugger;
                        }
                    }
                    CloseHandle(parentProcessHandle);
                }
                return false;
            }
        }

        /**
         * @brief 反调试核心逻辑：循环检测调试器，检测到则终止进程
         */
        void ADCL::antiDebug() {
#ifdef _DEBUG
            return;
#endif
            while (!m_stopFlag.load()) {
                bool isDetected = false;
                UINT_PTR peb = getPeb();

                // 多维度调试器检测
                if (IsDebuggerPresent()) {
                    isDetected = true;
                }
                if (peb && *(BYTE*)(peb + 2)) {
                    isDetected = true;
                }
                if (checkDebugPort()) {
                    isDetected = true;
                }
                if (checkDr()) {
                    isDetected = true;
                }
                if (checkParentDebug()) {
                    isDetected = true;
                }

                // 检测到调试器则终止进程
                if (isDetected) {
                    std::lock_guard<std::mutex> lock(m_mtx);
                    m_debugger = true;
                    killProcess();
                    return;
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(97));
            }
        }

#ifdef _MSC_VER
#pragma warning(pop)
#endif
    }
}
#else
#pragma message("Because Scriptforge.AntiDebug.ixx is only compatible with Windows,it will not be compiled.")
#endif