#pragma once
#ifndef SCRIPTFORGETREE_HPP
#define SCRIPTFORGETREE_HPP
#include <memory>
#include <vector>

namespace Scriptforge::Tree {

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
        using const_iterator = int;
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
#endif // !SCRIPTFORGETREE_HPP