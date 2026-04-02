<center>
  <img src="src/ScriptForge.png" alt="Scriptforge" width="100"/>
  <h1>Scriptforge</h1>
  <h2>一个有着许许多多奇奇怪怪功能的开源库</h2>

[README主版本(English)](README.md)

[更新日志](CHANGELOG-CN.md)

</center>

# ⚠️ 维护通知

本库目前正在进行重大重构和更新。
请注意：

- **编译错误**：当前版本可能会出现编译错误
- **API稳定性**：在此期间无法保证API稳定性
- **部分功能**：可能暂时不可用

我们建议在更新完成前使用最新的稳定版本。

**预计完成时间**：2026年第二季度


### 🎉 项目重新开始维护！


---


**Scriptforge\:\:Err\:\:Error 错误码对应：**
- Tree0001 [函数]:Node not found
- Tree0002 [函数]:Empty node  
- Tree0003 [函数]:Orphaned node
- BitPack0001 [函数]:size must be 8.
- BitPack0002 [函数]:where must be less than 8.

---



### 项目结构：
```
Scriptforge/
│   .gitattributes
│   .gitignore
│   CHANGELOG-CN.md
│   CHANGELOG.md
│   LICENSE-THIRD-PARTY.txt
│   LICENSE.txt
│   README-CN.md
│   README.md
│   Scriptforge.sln
│   Scriptforge.slnLaunch.user
│   更新方向.md
│
├───ScriptforgeLib
│   │   json.hpp
│   │   Scriptforge.AntiDebug.ixx
│   │   Scriptforge.BitPack.ixx
│   │   Scriptforge.Err.ixx
│   │   ScriptForge.ErrCode.ixx
│   │   Scriptforge.ErrCode.throwError.ixx
│   │   Scriptforge.ixx
│   │   Scriptforge.LanguageCode.ixx
│   │   Scriptforge.Local.ixx
│   │   Scriptforge.Log.ixx
│   │   Scriptforge.Msg.ixx
│   │   Scriptforge.RingBuffer.ixx
│   │   Scriptforge.ThreadError.ixx
│   │   Scriptforge.Tree.ixx
│   │   Scriptforge.Version.ixx
│   │   ScriptforgeLib.vcxproj
│   │   ScriptforgeLib.vcxproj.filters
│   │   ScriptforgeLib.vcxproj.user
│   │   统计代码行数.ps1
│   │
│   └───lang
│            en.json
│            zh.json
│
├───ScriptforgeTest
│        LogTest.cpp
│        LogTest.h
│        ScriptforgeTest.vcxproj
│        ScriptforgeTest.vcxproj.filters
│        ScriptforgeTest.vcxproj.user
│        Test.cpp
│        Test.h
│        TreeTest.cpp
│        TreeTest.h
│   
└───src
         ScriptForge.png
```
---

### 📝 使用说明
- 目前使用 Visual Studio sln 项目开发
- 如需 CMake 构建，请自行配置 CMakeLists.txt 文件
- 更多构建方式正在规划中...