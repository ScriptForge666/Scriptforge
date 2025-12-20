// Copyright 2025 Scriptforge
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//     http://www.apache.org/licenses/LICENSE-2.0
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
export module Scriptforge.Tree;
import std;
namespace Scriptforge::Tree {
    export enum class TreeTraversalOrder {
        PreOrder,
        PostOrder,
        LevelOrder
    };
    export
    template <typename TreeType>
    class ConstTreeIterator {
    public:
        using value_type = typename TreeType::value_type;
        using reference = const value_type&;
        using pointer = const value_type*;
        using size_type = std::size_t;
        using difference_type = std::ptrdiff_t;
        using iterator_category = std::forward_iterator_tag;
        ConstTreeIterator() = default;
        explicit ConstTreeIterator(typename TreeType::nodeptr node, TreeTraversalOrder order = TreeTraversalOrder::LevelOrder);
        reference operator*() const;
        pointer operator->() const;
        ConstTreeIterator& operator++();
        ConstTreeIterator operator++(int);
        bool operator==(const ConstTreeIterator& other) const;
        bool operator!=(const ConstTreeIterator& other) const;
        TreeType::nodeptr current_node() const;
    private:
        typename TreeType::nodeptr m_current_node;
        TreeTraversalOrder m_traversal_order;
        // 私有辅助函数
        void advance_preorder();
        void advance_postorder();
        void advance_levelorder();
        bool has_children() const;
        void go_to_first_child();
        void find_next_sibling_or_ancestor();
        bool move_to_next_sibling(const typename TreeType::nodeptr& current,
            const typename TreeType::nodeptr& father);
    };

    export
    template<typename T, typename Alloc = std::allocator<T>>
        requires requires(T t1, T t2) { t1 = t2; }
    class Tree {
    private:
        struct TreeNode;
    public:
        using value_type = T;
        using reference = value_type&;
        using const_reference = const value_type&;
        using size_type = std::size_t;
        using iterator = int;
        using const_iterator = ConstTreeIterator;
        using difference_type = std::ptrdiff_t;
        using allocator_type = Alloc;
        using nodeptr = std::shared_ptr<TreeNode>;

        //构造函数
        Tree(const allocator_type& alloc = allocator_type());
        explicit Tree(const T& node, const allocator_type& alloc = allocator_type());
        Tree(const Tree<T, Alloc>& other);

        nodeptr root() const;                              //返回根节点
        nodeptr del(nodeptr node);                         //删除节点

        //添加节点
        nodeptr add(nodeptr father);
        nodeptr add(nodeptr father, T& node);
        nodeptr add(nodeptr father, const T& node);

        allocator_type get_allocator() const noexcept;     //返回分配器

    private:
        struct TreeNode
        {
            TreeNode() = default;
            explicit TreeNode(T& v) : node(v) {}
            explicit TreeNode(const T& v) : node(v) {}
            std::weak_ptr<TreeNode> father;

            std::vector<nodeptr, typename std::allocator_traits<Alloc>::template rebind_alloc<nodeptr>> children;
            T node;
        };

        nodeptr m_root;
        allocator_type alloc_;

        template<typename U>
        nodeptr create_node(U&& value);
    };
}

module :private;
import Scriptforge.Err;

namespace Scriptforge::Tree {
    //ConstTreeIterator实现部分

   //构造函数
    template<typename TreeType>
    ConstTreeIterator<TreeType>::ConstTreeIterator(typename TreeType::nodeptr node, TreeTraversalOrder order)
        : m_current_node(node), m_traversal_order(order) {
        // 初始化逻辑（如果需要）
    }
    //解引用操作符
    template<typename TreeType>
    typename ConstTreeIterator<TreeType>::reference
        ConstTreeIterator<TreeType>::operator*() const {
        return m_current_node->node;
    }
    //箭头操作符
    template<typename TreeType>
    typename ConstTreeIterator<TreeType>::pointer
        ConstTreeIterator<TreeType>::operator->() const {
        return &(m_current_node->node);
    }
    //前置递增操作符
    template<typename TreeType>
    ConstTreeIterator<TreeType>&
        ConstTreeIterator<TreeType>::operator++() {
        switch (m_traversal_order) {
        case TreeTraversalOrder::PreOrder:
            advance_preorder();
            break;
        case TreeTraversalOrder::PostOrder:
            advance_postorder();
            break;
        case TreeTraversalOrder::LevelOrder:
            advance_levelorder();
            break;
        }
        return *this;
    }
    //后置递增操作符
    template<typename TreeType>
    ConstTreeIterator<TreeType>
        ConstTreeIterator<TreeType>::operator++(int) {
        ConstTreeIterator temp = *this;
        ++(*this);
        return temp;
    }
    //相等比较操作符
    template<typename TreeType>
    bool ConstTreeIterator<TreeType>::operator==(const ConstTreeIterator& other) const {
        return m_current_node == other.m_current_node;
    }
    //不等比较操作符
    template<typename TreeType>
    bool ConstTreeIterator<TreeType>::operator!=(const ConstTreeIterator& other) const {
        return !(*this == other);
    }
    //返回树指针
    template<typename TreeType>
    TreeType::nodeptr ConstTreeIterator<TreeType>::current_node() const {
        return m_current_node;
    }
    //私有辅助函数
    template<typename TreeType>
    void ConstTreeIterator<TreeType>::advance_preorder() {
        if (!m_current_node) return;

        if (has_children()) {
            go_to_first_child();
        }
        else {
            find_next_sibling_or_ancestor();
        }
    }
    template<typename TreeType>
    bool ConstTreeIterator<TreeType>::has_children() const {
        return !m_current_node->children.empty();
    }
    template<typename TreeType>
    void ConstTreeIterator<TreeType>::go_to_first_child() {
        m_current_node = m_current_node->children.front();
    }
    template<typename TreeType>
    void ConstTreeIterator<TreeType>::find_next_sibling_or_ancestor() {
        auto temp = m_current_node;
        while (temp) {
            auto father = temp->father.lock();
            if (!father) {
                m_current_node = nullptr;
                return;
            }

            if (move_to_next_sibling(temp, father)) {
                break;
            }

            temp = father; // 继续向上回溯
        }

        if (!temp) {
            m_current_node = nullptr;
        }
    }
    template<typename TreeType>
    bool ConstTreeIterator<TreeType>::move_to_next_sibling(const typename TreeType::nodeptr& current,
        const typename TreeType::nodeptr& father) {
        auto& siblings = father->children;
        auto it = std::find(siblings.begin(), siblings.end(), current);

        if (it != siblings.end() && ++it != siblings.end()) {
            m_current_node = *it;
            return true;
        }
        return false;
    }

    template<typename TreeType>
    void ConstTreeIterator<TreeType>::advance_postorder() {
        if (!m_current_node) return;

        auto temp = m_current_node;
        while (temp) {
            auto father = temp->father.lock();
            if (!father) {
                // 到达根节点，遍历结束
                m_current_node = nullptr;
                return;
            }

            auto& siblings = father->children;
            auto it = std::find(siblings.begin(), siblings.end(), temp);
            if (it != siblings.end()) {
                ++it; // 移动到下一个兄弟
                if (it != siblings.end()) {
                    // 找到下一个兄弟，需要找到该兄弟子树的最左节点
                    m_current_node = *it;
                    // 一直往下找最左边的叶子节点
                    while (!m_current_node->children.empty()) {
                        m_current_node = m_current_node->children.front();
                    }
                    return;
                }
                else {
                    // 当前是父节点的最后一个子节点，访问父节点
                    m_current_node = father;
                    return;
                }
            }
            temp = father;
        }

        // 遍历完成
        m_current_node = nullptr;
    }

    template<typename TreeType>
    void ConstTreeIterator<TreeType>::advance_levelorder() {
        if (!m_current_node) return;

        // 层序遍历需要队列来维护访问顺序
        // 由于迭代器的限制，我们用一种简化的方式来模拟

        auto temp = m_current_node;

        // 首先检查是否有子节点
        if (!temp->children.empty()) {
            m_current_node = temp->children.front();
            return;
        }

        // 没有子节点，寻找兄弟节点
        auto father = temp->father.lock();
        if (father) {
            auto& siblings = father->children;
            auto it = std::find(siblings.begin(), siblings.end(), temp);
            if (it != siblings.end() && ++it != siblings.end()) {
                m_current_node = *it;
                return;
            }
        }

        // 没有兄弟节点，需要更复杂的层序逻辑
        // 这里用简化版本：直接结束
        m_current_node = nullptr;
    }

    //Tree<T>实现部分

        //构造函数
    template<typename T, typename Alloc>
        requires requires(T t1, T t2) { t1 = t2; }
    Tree<T, Alloc>::Tree(const allocator_type& alloc)
        : alloc_(alloc) {
        m_root = create_node(T());
    }

    template<typename T, typename Alloc>
        requires requires(T t1, T t2) { t1 = t2; }
    Tree<T, Alloc>::Tree(const T& node, const allocator_type& alloc)
        : alloc_(alloc) {
        m_root = create_node(node);
    }

    template<typename T, typename Alloc>
        requires requires(T t1, T t2) { t1 = t2; }
    Tree<T, Alloc>::Tree(const Tree<T, Alloc>& other)
        : alloc_(std::allocator_traits<allocator_type>::select_on_container_copy_construction(other.alloc_)) {
        auto deep_copy = [&](auto&& self, const nodeptr& original, const nodeptr& parent) -> nodeptr {
            if (!original) return nullptr;
            nodeptr copy_node = create_node(original->node);
            copy_node->father = parent;
            for (const auto& child : original->children) {
                auto child_copy = self(self, child, copy_node);
                if (child_copy) {
                    copy_node->children.push_back(child_copy);
                }
            }
            return copy_node;
            };
        m_root = deep_copy(deep_copy, other.m_root, nullptr);
    }

    //返回根节点
    template<typename T, typename Alloc>
        requires requires(T t1, T t2) { t1 = t2; }
    typename Tree<T, Alloc>::nodeptr Tree<T, Alloc>::root() const {
        return m_root;
    }

    //删除节点
    template<typename T, typename Alloc>
        requires requires(T t1, T t2) { t1 = t2; }
    typename Tree<T, Alloc>::nodeptr Tree<T, Alloc>::del(nodeptr node) {
        if (!node) throw Scriptforge::Err::Error{ "Tree0002", std::string(__func__) + "Empty node" };
        nodeptr father = node->father.lock();
        if (!father) {
            if (node != m_root) throw Scriptforge::Err::Error{ "Tree0003", std::string(__func__) + "Orphaned node" };
            else m_root.reset();
            return nullptr;
        }
        auto& vec = father->children;
        auto it = std::find(vec.begin(), vec.end(), node);
        if (it == vec.end()) throw Scriptforge::Err::Error{ "Tree0001", std::string(__func__) + "Node not found" };

        vec.erase(it);
        return father;
    }

    //添加节点
    template<typename T, typename Alloc>
        requires requires(T t1, T t2) { t1 = t2; }
    typename Tree<T, Alloc>::nodeptr Tree<T, Alloc>::add(nodeptr father) {
        if (!father) throw Scriptforge::Err::Error{ "Tree0002", std::string(__func__) + "Empty node" };
        nodeptr newnode = create_node(T());
        newnode->father = father;
        father->children.push_back(newnode);
        return newnode;
    }

    template<typename T, typename Alloc>
        requires requires(T t1, T t2) { t1 = t2; }
    typename Tree<T, Alloc>::nodeptr Tree<T, Alloc>::add(nodeptr father, T& node) {
        if (!father) throw Scriptforge::Err::Error{ "Tree0002", std::string(__func__) + "Empty node" };
        nodeptr newnode = create_node(node);
        newnode->father = father;
        father->children.push_back(newnode);
        return newnode;
    }

    template<typename T, typename Alloc>
        requires requires(T t1, T t2) { t1 = t2; }
    typename Tree<T, Alloc>::nodeptr Tree<T, Alloc>::add(nodeptr father, const T& node) {
        if (!father) throw Scriptforge::Err::Error{ "Tree0002", std::string(__func__) + "Empty node" };
        nodeptr newnode = create_node(node);
        newnode->father = father;
        father->children.push_back(newnode);
        return newnode;
    }

    //返回分配器
    template<typename T, typename Alloc>
        requires requires(T t1, T t2) { t1 = t2; }
    typename Tree<T, Alloc>::allocator_type Tree<T, Alloc>::get_allocator() const noexcept {
        return alloc_;
    }

    //创建新节点
    template<typename T, typename Alloc>
        requires requires(T t1, T t2) { t1 = t2; }
    template<typename U>
    typename Tree<T, Alloc>::nodeptr
        Tree<T, Alloc>::create_node(U&& value) {
        using node_allocator = typename std::allocator_traits<Alloc>
            ::template rebind_alloc<TreeNode>;
        using node_alloc_traits = std::allocator_traits<node_allocator>;
        node_allocator node_alloc(alloc_);
        TreeNode* raw = node_alloc.allocate(1);
        try {
            node_alloc_traits::construct(node_alloc, raw, std::forward<U>(value));
        }
        catch (...) {
            node_alloc.deallocate(raw, 1);
            throw;
        }
        return nodeptr(raw, [&node_alloc](TreeNode* p) noexcept {
            if (p) {
                node_alloc_traits::destroy(node_alloc, p);
                node_alloc.deallocate(p, 1);
            }
            });
    }
}