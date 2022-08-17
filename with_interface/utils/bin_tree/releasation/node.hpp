#pragma once

#include "../bin_tree_def.hpp"

template <typename Key, typename Additional, typename Data>
class bin_tree<Key, Additional, Data>::node {
public:
	node *left = nullptr, *right = nullptr, *ancestor = nullptr;
private:
	utils::triple<Key, Additional, Data> kern;
public:
	node(const utils::triple<Key, Additional, Data> &kern) : kern(kern) {}

	node(const Key &x, const Additional &y, const Data &z) :
		kern(utils::make_triple(x, y, z)) {}

	node(const Key &x, const Additional &y, const Data &z,
		node *ancestor) :
		kern(utils::make_triple(x, y, z)),
		ancestor(ancestor) {}

	node(const Key &x, const Additional &y, const Data &z,
		node *left, node *right) :
		kern(utils::make_triple(x, y, z)),
		left(left), right(right) {}

	node(const Key &x, const Additional &y, const Data &z,
		node *left, node *right, node *ancestor) :
		kern(utils::make_triple(x, y, z)),
		left(left), right(right), ancestor(ancestor) {}

	~node() {
		delete left;
		delete right;
	}

	decltype(kern) *operator->() { return &kern; }
	decltype(kern) &operator*() { return kern; }

	virtual void print(std::ostream &out) {
		out << "[" << kern.first << " (" << kern.second << ")] = " <<
			kern.third;
	}
};
