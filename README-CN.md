# Scriptforge

### 🎉 项目重新开始维护！

**一个有着许许多多奇奇怪怪功能的开源库**

---


**Scriptforge::Err::Error 错误码对应：**
- E0001 未发现节点
- E0002 空节点  
- E0003 孤立节点

---
### 项目结构：
```
Scriptforge/
├── Scriptforge.sln
├── README.md
├──README-CN.md
├── ScriptforgeLib/
│    ├── Scriptforge.ixx
│    ├── Scriptforge.BitPack.ixx
│    ├── Scriptforge.Err.ixx
│    ├── Scriptforge.Log.ixx
│    ├── Scriptforge.Tree.ixx
│    ├── Scriptforge.Version.ixx
│    ├── Scriptforge.BitPack.cpp
│    ├── Scriptforge.Err.cpp
│    ├── Scriptforge.Log.cpp
│    ├── Scriptforge.Tree.cpp
│    └──Scriptforge.Version.cpp
└── ScriptforgeTest/
        └── ScriptforgeTest.cpp

```
---

### 📝 使用说明
- 目前使用 Visual Studio sln 项目开发
- 如需 CMake 构建，请自行配置 CMakeLists.txt 文件
- 更多构建方式正在规划中...
