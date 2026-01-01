<center>
  <img src="src/ScriptForge.png" alt="Scriptforge" width="100"/>
  <h1>Scriptforge</h1>
  <h2>An open-source library with many strange and wonderful features</h2>

**Other Language:** [Chinese](README-CN.md)

[Changelog](CHANGELOG.md)

</center>




### 🎉 The project is back under active development!

---

**Scriptforge\:\:Err\:\:Error error codes:**
- Tree0001 [Function]:Node not found
- Tree0002 [Function]:Empty node  
- Tree0003 [Function]:Orphaned node
- BitPack0001 [Function]:size must be 8.
- BitPack0002 [Function]:where must be less than 8.

---
### Project Structure:
```
Scriptforge/
├── Scriptforge.sln
├── README.md
├── README-CN.md
├── UpdateLog.md
├── UpdateLog-CN.md
├── ScriptforgeLib/
│    ├── Scriptforge.ixx
│    ├── Scriptforge.AntiDebug.ixx
│    ├── Scriptforge.BitPack.ixx
│    ├── Scriptforge.Err.ixx
│    ├── Scriptforge.Log.ixx
│    ├── Scriptforge.Tree.ixx
│    └── Scriptforge.Version.ixx
├── ScriptforgeTest/
│    └── ScriptforgeTest.cpp
└── src/

```
---

### 📝 Usage Instructions
- Currently developed using Visual Studio solution projects
- For CMake builds, please configure your own CMakeLists.txt file
- More build methods are being planned...