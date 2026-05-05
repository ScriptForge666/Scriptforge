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

 我来**逐行、逐句、逐细节**为你彻底解释这段代码，它是**C++ 树类的拷贝构造函数**，核心作用是**深拷贝整棵树**。

先给你整体定位：
- 这是 `Tree` 类的**拷贝构造函数**
- 功能：把另一个树 `other` 完整复制一份**新树**（深拷贝，不共享内存）
- 里面用到了**递归 lambda**（C++14 及以上）实现树的深度复制

---

# 逐行详细解释

```cpp
template<typename T, typename Alloc>
```
- 这是**模板声明**
- 表示这个类是模板类：
  - `T`：树节点存储的数据类型
  - `Alloc`：内存分配器（类似 `std::vector` 的分配器）

```cpp
Tree<T, Alloc>::Tree(const Tree<T, Alloc>& other)
```
- **拷贝构造函数**
- 参数：`const` 引用的另一个树 `other`
- 作用：用已有的树创建一个**全新、独立**的树

```cpp
    : m_alloc(other.getAllocator()), m_lang(other.getLang()) {
```
- **初始化列表**
- 直接复制**简单成员变量**：
  - `m_alloc`：内存分配器（分配器可以直接复制）
  - `m_lang`：语言/标记之类的成员（简单值，直接复制）
- 这些是**浅拷贝没问题**的成员，不需要深拷贝

```cpp
    auto deepCopy = [&](auto&& self, const nodeptr& original, const nodeptr& parent) -> nodeptr {
```
- 定义一个**递归 lambda 函数**，名字叫 `deepCopy`
- 作用：**递归深拷贝一棵树的所有节点**

### lambda 参数解释：
1. `[&]`：捕获外部所有变量（能访问 `this`、`m_alloc`、`other`）
2. `auto&& self`：**lambda 递归必须的自引用**（C++14 泛型 lambda 技巧）
3. `const nodeptr& original`：当前要复制的**原节点**
4. `const nodeptr& parent`：复制后新节点的**父节点**
5. `-> nodeptr`：返回复制好的**新节点指针**

```cpp
        if (!original) return nullptr;
```
- **递归终止条件**
- 如果原节点是空（nullptr），直接返回空
- 没有节点就不需要复制

```cpp
        nodeptr copy_node = create_node(original->node);
```
- **创建新节点**
- 调用 `create_node` 工厂函数
- 把原节点的数据 `original->node` 复制到新节点
- `copy_node` 是**全新的节点**，和原节点不共享内存

```cpp
        copy_node->father = parent;
```
- 设置新节点的**父指针**
- 让新节点知道自己的父亲是谁
- 维护树的父子关系

```cpp
        for (const auto& child : original->children) {
```
- 遍历**原节点的所有子节点**
- 树的每个节点可能有多个孩子，必须全部复制

```cpp
            auto child_copy = self(self, child, copy_node);
```
- **递归调用 lambda 自己**
- `self(self, ...)` 是 lambda 递归的固定写法
- 把当前子节点 `child` 复制一份
- 新子节点的父亲是 `copy_node`

```cpp
            if (child_copy) {
                copy_node->children.push_back(child_copy);
            }
```
- 如果复制出了有效子节点
- 就把它加入到**新节点的子节点列表**
- 完成父子关系重建

```cpp
        }
        return copy_node;
```
- 循环结束
- 返回复制完成的**新节点**（带所有后代）
- 这一层递归完成

```cpp
		};
```
- lambda 函数定义结束
- ⚠️ **重要：你贴的代码这里不完整**
- 定义完 `deepCopy` 后**必须调用它**，否则不会真的复制树

---

# 这段代码缺了什么？（非常关键）
你贴的代码**只定义了复制函数，但没有调用**。

完整、正确的代码必须在 `};` 后面加一句：
```cpp
// 调用递归函数，从根节点开始复制
m_root = deepCopy(deepCopy, other.m_root, nullptr);
```

否则：**lambda 定义了但永远不执行，树没有被复制！**

---

# 极简总结（帮助你快速记忆）
1. 这是**树的拷贝构造函数**
2. 先复制简单成员（分配器、语言标记）
3. 定义**递归 lambda** 做**树的深拷贝**
4. 递归逻辑：
   - 节点为空 → 返回空
   - 复制当前节点
   - 递归复制所有子节点
   - 重建父子关系
5. 最后必须**调用 lambda** 才会真正复制树

---

### 总结
- 这是**Tree 类的拷贝构造函数**，用于**深拷贝**一棵树。
- 初始化列表复制**非节点成员**（分配器、标记）。
- 递归 lambda 负责**复制每一个节点 + 重建父子关系**。
- 你的代码**缺少最后调用 lambda** 的关键行。

如果你愿意，我可以帮你**补全完整可运行的代码**。