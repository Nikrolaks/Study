#pragma once

#include "../bin_tree_def.hpp"
#include "node.hpp"

template <typename Key>
typename bin_tree<Key>::node *bin_tree<Key>::build_tree(
	const std::vector<node *> &nodes,
	std::size_t left, std::size_t right) {
	if (left >= right) {
		return nullptr;
	}

	std::size_t mid = (left + right) >> 1;
	node *res = nodes[mid];
	bind<'l'>(res, build_tree(nodes, left, mid));
	bind<'r'>(res, build_tree(nodes, mid + 1, right));

	return res;
}
