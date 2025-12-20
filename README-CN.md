# Scriptforge

**[README主版本(English)](README.md)**

[更新日志](UpdateLog-CN.md)

## 停止更新 Header 分支，仅维护 Module 分支

尊敬的用户、合作伙伴及贡献者：

自 **2025 年 12 月 20日** 起，本库将 **停止对 `header`（头文件）分支的任何功能性更新、改进或新特性**，后续只在 `module`（模块）分支上进行持续维护与发布。  

### 主要原因
1. **技术趋势**：C++20/23 的模块系统已成为业界标准，能够显著提升编译速度、二进制体积以及依赖管理的可靠性。  
2. **维护成本**：保留两套并行的实现（Header + Module）在资源投入、文档同步和错误修复方面成本过高。  
3. **维护质量**：集中精力在 Module 分支上可以更快响应用户需求、提供更完整的测试与安全保障。

### 受影响范围
- **`header` 分支** 将仍然保持 **只读**（仅接受关键的安全补丁），不再接受功能 PR、性能优化或新 API。  
- 所有 **Issue**、**Pull Request** 仍然可以在 `header` 分支上提交，但团队会建议在 `module` 分支上讨论与实现。  
- **已发布的旧版本**（基于 Header）仍可继续使用，只是将不再提供后续功能更新。

我们衷心感谢大家一路以来的支持与贡献，期待与您一起在更现代、更高效的 C++ 模块体系中共创未来！  

**—— Scriptforge 开发团队**  
2025‑12‑20  

---

### 🎉 项目重新开始维护！

**一个有着许许多多奇奇怪怪功能的开源库**

---


- Tree0001 Node not found
- Tree0002 Empty node  
- Tree0003 Orphaned node

---
### 项目结构：
```
Scriptforge/
├── Scriptforge.sln
├── README.md
├──README-CN.md
├── ScriptforgeLib/
│    ├── Scriptforge.hpp
│    ├── ScriptforgeBitPack.hpp(haven't had that)
│    ├── ScriptforgeErr.hpp
│    ├── ScriptforgeLog.hpp
│    ├── ScriptforgeTree.hpp
│    ├── ScriptforgeVersion.hpp
│    ├── ScriptforgeBitPack.cpp(haven't had that)
│    ├── ScriptforgeErr.cpp
│    ├── ScriptforgeLog.cpp
│    ├── ScriptforgeTree.cpp
│    └──ScriptforgeVersion.cpp
└── ScriptforgeTest/
        └── ScriptforgeTest.cpp

```
---

### 📝 使用说明
- 目前使用 Visual Studio sln 项目开发
- 如需 CMake 构建，请自行配置 CMakeLists.txt 文件
- 更多构建方式正在规划中...
