# Scriptforge

**Other Language:** [Chinese](README-CN.md)

[Changelog](UpdateLog.md)

## Discontinuing Updates to the Header Branch – Only the Module Branch Will Be Maintained

Dear users, partners, and contributors,

Effective **December 20 2025**, this library will **stop all functional updates, improvements, or new features on the `header` (header‑only) branch**. Going forward, development, maintenance, and releases will be carried out exclusively on the `module` (C++‑module) branch.

### Reasons

1. **Technical trend** – The C++20/23 module system has become the industry standard, offering substantial gains in compilation speed, binary size, and dependency‑management reliability.  
2. **Maintenance cost** – Keeping two parallel implementations (Header + Module) incurs excessive effort in resource allocation, documentation synchronization, and bug fixing.  
3. **Quality of upkeep** – Focusing on the Module branch enables us to respond faster to user needs, provide more comprehensive testing, and ensure stronger security guarantees.

### Scope of Impact

- The **`header` branch** will remain **read‑only** (only critical security patches may be back‑ported). No functional pull requests, performance optimisations, or new APIs will be accepted.  
- All **issues** and **pull requests** may still be opened against the `header` branch, but the team will recommend discussing and implementing them on the `module` branch instead.  
- **Previously released versions** built from the Header branch can continue to be used, but they will no longer receive functional updates.

We sincerely thank everyone for the support and contributions over the years and look forward to building a more modern, efficient C++ module ecosystem together with you!

**— The Scriptforge Development Team**  
2025‑12‑20

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

### 📝 Usage Instructions
- Currently developed using Visual Studio solution projects
- For CMake builds, please configure your own CMakeLists.txt file
- More build methods are being planned...