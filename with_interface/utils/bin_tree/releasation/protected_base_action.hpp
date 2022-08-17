#pragma once

#include "../bin_tree_def.hpp"
#include "node.hpp"

template<typename Key, typename Additional, typename Data>
void bin_tree<Key, Additional, Data>::insert(node *v) {
	auto ppos = find((*v)->first, &head, false);
	if (*(ppos.first))
		return;
	bind(find_ancestor(ppos.first, ppos.second),
		v, ppos.second);
}

template <typename Key, typename Additional, typename Data>
const std::pair<
	typename bin_tree<Key, Additional, Data>::node *,
	typename bin_tree<Key, Additional, Data>::node *>
	bin_tree<Key, Additional, Data>::split(
		typename bin_tree<Key, Additional, Data>::node *v, Key c) {
	if (!v)
		return std::make_pair(nullptr, nullptr);
	if ((*v)->first > c) {
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
