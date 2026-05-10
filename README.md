<center>
 
 <img src="src/ScriptForge.png" alt="Scriptforge" width="100"/>
 
 <h1>Scriptforge</h1>
 
 <h2>An open-source library with many strange and wonderful features</h2>

**Other Language:** [Chinese](README-CN.md)

[Changelog](CHANGELOG.md)

</center>

# ⚠️ Maintenance Notice

The library is currently undergoing a major refactoring and update. 
Please note that:

- **Compilation errors** may occur in the current version
- **API stability** is not guaranteed during this period  
- **Some features** may be temporarily unavailable

We recommend using the latest stable release until the update is complete.


### 🎉 The project is back under active development!

---
### Project Structure:
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

### 📝 Usage Instructions
- Currently developed using Visual Studio solution projects
- For CMake builds, please configure your own CMakeLists.txt file
- More build methods are being planned...