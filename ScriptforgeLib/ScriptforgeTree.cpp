#include "ScriptforgeTree.hpp"
#include "ScriptforgeErr.hpp"
#include <algorithm>
#include <memory>
#include <vector>

namespace Scriptforge::Tree {
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
        if (!node) throw Scriptforge::Err::Error{ "E0002", "空节点" };
        nodeptr father = node->father.lock();
        if (!father) {
            if (node != m_root) throw Scriptforge::Err::Error{ "E0003", "孤立节点" };
            else m_root.reset();
            return nullptr;
        }
        auto& vec = father->children;
        auto it = std::find(vec.begin(), vec.end(), node);
        if (it == vec.end()) throw Scriptforge::Err::Error{ "E0001", "未发现节点" };

        vec.erase(it);
        return father;
    }

    //添加节点
    template<typename T, typename Alloc>
        requires requires(T t1, T t2) { t1 = t2; }
    typename Tree<T, Alloc>::nodeptr Tree<T, Alloc>::add(nodeptr father) {
        if (!father) throw Scriptforge::Err::Error{ "E0002", "空节点" };
        nodeptr newnode = create_node(T());
        newnode->father = father;
        father->children.push_back(newnode);
        return newnode;
    }

    template<typename T, typename Alloc>
        requires requires(T t1, T t2) { t1 = t2; }
    typename Tree<T, Alloc>::nodeptr Tree<T, Alloc>::add(nodeptr father, T& node) {
        if (!father) throw Scriptforge::Err::Error{ "E0002", "空节点" };
        nodeptr newnode = create_node(node);
        newnode->father = father;
        father->children.push_back(newnode);
        return newnode;
    }

    template<typename T, typename Alloc>
        requires requires(T t1, T t2) { t1 = t2; }
    typename Tree<T, Alloc>::nodeptr Tree<T, Alloc>::add(nodeptr father, const T& node) {
        if (!father) throw Scriptforge::Err::Error{ "E0002", "空节点" };
        nodeptr newnode = create_node(node);
        newnode->father = father;
        father->children.push_back(newnode);
        return newnode;
    }

    //返回分配器
    template<typename T, typename Alloc>
        requires requires(T t1, T t2) { t1 = t2; }
    typename Tree<T,Alloc>::allocator_type Tree<T,Alloc>::get_allocator() const noexcept { 
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
    template class Tree<int>;

}