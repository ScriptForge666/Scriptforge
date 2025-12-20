# Scriptforge

**Other Language:** [Chinese](README-CN.md)

[Changelog](UpdateLog.md)

## <span style="color:#ff8c00;"> ⚠️ Warning – A major update is arriving soon. Please read the changelog carefully before upgrading.  </span>

### 🎉 The project is back under active development!

**An open-source library with many strange and wonderful features**

---

**Scriptforge::Err::Error error codes:**
- Tree0001 Node not found
- Tree0002 Empty node  
- Tree0003 Orphaned node

---
### Project Structure:
```
Scriptforge/
├── Scriptforge.sln
├── README.md
├── README-CN.md
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
│    └── Scriptforge.Version.cpp
└── ScriptforgeTest/
        └── ScriptforgeTest.cpp

```
---

### 📝 Usage Instructions
- Currently developed using Visual Studio solution projects
- For CMake builds, please configure your own CMakeLists.txt file
- More build methods are being planned...