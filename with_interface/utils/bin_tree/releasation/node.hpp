#pragma once

#include "../bin_tree_def.hpp"

template <typename Key>
class bin_tree<Key>::node {
public:
	node *left = nullptr, *right = nullptr, *ancestor = nullptr;
protected:
	Key key;
public:
	node(const Key &key) : key(key) {}

	node(const Key &key, node *ancestor) :
		key(key), ancestor(ancestor) {}

	node(const Key &key, node *left, node *right) :
		key(key), left(left), right(right) {}

	node(const Key &key, node *left, node *right, node *ancestor) :
		key(key), left(left), right(right), ancestor(ancestor) {}

	~node() {
		delete left;
		delete right;
	}

	const Key &operator*() { return key; }

	virtual void print(std::ostream &out) {
		out << "[" << key << "]";
	}
};
