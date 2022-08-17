#pragma once

#include "../bin_tree_def.hpp"
#include "node.hpp"

template<typename Key>
void bin_tree<Key>::insert(node *v) {
	auto ppos = find(**v, &head, false);
	if (*(ppos.first))
		return;
	bind(find_ancestor(ppos.first, ppos.second),
		v, ppos.second);
}

template <typename Key>
const std::pair<
		typename bin_tree<Key>::node *,
		typename bin_tree<Key>::node *
	> bin_tree<Key>::split(node *v, Key c) {
	if (!v)
		return std::make_pair(nullptr, nullptr);
	if (**v > c) {
		auto p(split(v->left, c));
		bind<'l'>(v, p.second);
		return std::make_pair(p.first, v);
	}
	else {
		auto p(split(v->right, c));
		bind<'r'>(v, p.first);
		return std::make_pair(v, p.second);
	}
}
