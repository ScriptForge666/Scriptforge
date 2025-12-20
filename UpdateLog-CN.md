# Scriptforge Library Changelog

[更新文件主版本(English)](UpdateLog.md)

# v0.1.0
所有实现文件已被移除并合并到模块文件中，从而消除了显式模板实例化的需要。

# v0.0.1
添加版本系统(文件：[Scriptforge.Version.cpp](ScriptforgeLib/Scriptforge.Version.cpp),[Scriptforge.Version.ixx](ScriptforgeLib/Scriptforge.Version.ixx))和它自身版本（在[Scriptforge.ixx](ScriptforgeLib/Scriptforge.ixx)中）

# v0.0.0(before v0.0.1)

 添加了初始项目结构和基础模块：
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
│    ├── Scriptforge.BitPack.cpp
│    ├── Scriptforge.Err.cpp
│    ├── Scriptforge.Log.cpp
│    └──Scriptforge.Tree.cpp
└── ScriptforgeTest/
        └── ScriptforgeTest.cpp
```