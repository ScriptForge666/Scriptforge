//Copyright 2025 Scriptforge
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//     http://www.apache.org/licenses/LICENSE-2.0
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
#include "ScriptforgeTree.hpp"
#include "ScriptforgeErr.hpp"
import std;
using namespace std;

namespace Scriptforge::Tree {
	//public
	template<typename T>requires requires(T t1, T t2) { t1 = t2; }
	Tree<T>::Tree() {
		m_root = make_shared<TreeNode>();
	}

	template<typename T>requires requires(T t1, T t2) { t1 = t2; }
	Tree<T>::Tree(const T& node) {
		m_root = make_shared<TreeNode>(node);
	}
	template<typename T>requires requires(T t1, T t2) { t1 = t2; }
	Tree<T>::Tree(const Tree<T>& other) {
			auto deep_copy = [&](auto&& self, const nodeptr& original, const nodeptr& parent) -> nodeptr {
			if (!original) return nullptr;
			nodeptr copy_node = make_shared<TreeNode>(original->node);
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

	template<typename T>requires requires(T t1, T t2) { t1 = t2; }
	Tree<T>::nodeptr Tree<T>::root() const{
		return m_root;
	}

	template<typename T>requires requires(T t1, T t2) { t1 = t2; }
	Tree<T>::nodeptr Tree<T>::del(nodeptr node) {
		if (!node) throw Scriptforge::Err::Error{ "E0002", "空节点" };
		Tree<T>::nodeptr father = node->father.lock();
		if (!father) {
			if (node != m_root) throw Scriptforge::Err::Error{ "E0003", "孤立节点" };
			else m_root.reset();
			return nullptr;
		}
		auto& vec = father->children;
		auto  it = std::find(vec.begin(), vec.end(), node);
		if (it == vec.end()) throw Scriptforge::Err::Error{ "E0001", "未发现节点" };

		vec.erase(it);
		return father;
	}

	template <typename T>requires requires(T t1, T t2) { t1 = t2; }
	Tree<T>::nodeptr Tree<T>::add(nodeptr father) {
		if (!father) throw Scriptforge::Err::Error{ "E0002", "空节点" };
		nodeptr newnode{ make_shared<TreeNode>() };
		auto& vec = father->children;
		vec.push_back(newnode);
		return newnode;
	}

	template <typename T>requires requires(T t1, T t2) { t1 = t2; }
	Tree<T>::nodeptr Tree<T>::add(nodeptr father, T& node) {
		if (!father) throw Scriptforge::Err::Error{ "E0002", "空节点" };
		nodeptr newnode{ make_shared<TreeNode>(node) };
		auto& vec = father->children;
		vec.push_back(newnode);
		return newnode;
	}

	template <typename T>requires requires(T t1, T t2) { t1 = t2; }
	Tree<T>::nodeptr Tree<T>::add(nodeptr father, const T& node) {
		if (!father) throw Scriptforge::Err::Error{ "E0002", "空节点" };
		nodeptr newnode{ make_shared<TreeNode>(node) };
		auto& vec = father->children;
		vec.push_back(newnode);
		return newnode;
	}
	//private
	template<typename T>requires requires(T t1, T t2) { t1 = t2; }
	Tree<T>::TreeNode::TreeNode(T& v) : node(v){}

	template<typename T>requires requires(T t1, T t2) { t1 = t2; }
	Tree<T>::TreeNode::TreeNode(const T& v) : node(v){}

	template class Tree<int>;
	
}
