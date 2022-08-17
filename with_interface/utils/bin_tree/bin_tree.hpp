#pragma once

#include <iostream>
#include <string>
#include <algorithm>

#include "bin_tree_def.hpp"
#include "releasation/node.hpp"
#include "releasation/base_action.hpp"
#include "releasation/private_base_action.hpp"
#include "releasation/protected_base_action.hpp"
#include "releasation/find.hpp"


template <typename Key, typename Additional, typename Data>
bin_tree<Key, Additional, Data>::bin_tree(
	std::vector<utils::triple<Key, Additional, Data>> &nodes) {
	std::sort(nodes.begin(), nodes.end(), 
		[](const decltype(nodes[0]) &f,
			const decltype(nodes[0]) &s) {
		return f.first < s.first;
	});
	head = build_tree(nodes, 0, nodes.size());
}

template <typename Key, typename Additional, typename Data>
void bin_tree<Key, Additional, Data>::display(std::ostream &out, node *it,
	char v_symb, std::string &&fmt, utils::cool_int deep) const {
	if (!it)
		return;
	if (it->right) {
		fmt.push_back((v_symb == char(132) ? char(129) : ' '));
		fmt.push_back(' ');
		display(out, it->right, char(130), std::move(fmt), deep + 1);
		fmt.pop_back();
		fmt.pop_back();
	}
	out << char(135) << deep.x << char(134) << fmt;
	out << v_symb << " ";
	it->print(out);
	out << std::endl;
	if (it->left) {
		fmt.push_back((v_symb == char(130) ? char(129) : ' '));
		fmt.push_back(' ');
		display(out, it->left, char(132), std::move(fmt), deep + 1);
		fmt.pop_back();
		fmt.pop_back();
	}
}
