# Scriptforge

[README主版本(English)](README.md)

[更新日志](UpdateLog-CN.md)

### 🎉 项目重新开始维护！

**一个有着许许多多奇奇怪怪功能的开源库**

---


**Scriptforge::Err::Error 错误码对应：**
- Tree0001 [函数]:Node not found
- Tree0002 [函数]:Empty node  
- Tree0003 [函数]:Orphaned node
- BitPack0001 [函数]:size must be 8.
- BitPack0002 [函数]:where must be less than 8.

---
### 项目结构：
```
Scriptforge/
├── Scriptforge.sln
├── README.md
├── README-CN.md
├── UpdateLog.md
├── UpdateLog-CN.md
├── ScriptforgeLib/
│    ├── Scriptforge.ixx
│    ├── Scriptforge.BitPack.ixx
│    ├── Scriptforge.Err.ixx
│    ├── Scriptforge.Log.ixx
│    ├── Scriptforge.Tree.ixx
│    └── Scriptforge.Version.ixx
└── ScriptforgeTest/
        └── ScriptforgeTest.cpp

```
---

### 📝 使用说明
- 目前使用 Visual Studio sln 项目开发
- 如需 CMake 构建，请自行配置 CMakeLists.txt 文件
- 更多构建方式正在规划中...
