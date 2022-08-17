#pragma once

#include "../bin_tree_def.hpp"
#include "node.hpp"

template <typename Key, typename Additional, typename Data>
typename bin_tree<Key, Additional, Data>::node *
bin_tree<Key, Additional, Data>::build_tree(
	const std::vector<
	utils::triple<Key, Additional, Data>> &nodes,
	std::size_t left, std::size_t right) {
	if (left >= right) {
		return nullptr;
	}
	std::size_t mid = (left + right) >> 1;
	node *res = new node(nodes[mid].first, nodes[mid].second, nodes[mid].third);
	bind<'l'>(res, build_tree(nodes, left, mid));
	bind<'r'>(res, build_tree(nodes, mid + 1, right));
	return res;
}
