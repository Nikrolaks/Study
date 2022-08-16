#pragma once

#include <iostream>
#include <vector>
#include <clocale>
#include <string>
#include <algorithm>

#include "treap_def.hpp"

template <typename Key, typename Prior, typename Data>
treap<Key, Prior, Data>::treap(
	std::vector<utils::triple<Key, Prior, Data>> &nodes) {
	std::sort(nodes.begin(), nodes.end(), [](
		const decltype(nodes[0]) &f,
		const decltype(nodes[0]) &s) {
		return f.first < s.first; 
	});
	if (nodes.size()) {
		bin_tree<Key, Prior, Data>::head = 
			new typename bin_tree<Key, Prior, Data>::node(
				nodes[0].first, nodes[0].second, nodes[0].third);
		typename bin_tree<Key, Prior, Data>::node *last = 
			bin_tree<Key, Prior, Data>::head;
		auto it = nodes.begin();
		for (++it; it != nodes.end(); ++it) {
			while (last && (*last)->second > (*it).second)
				last = last->ancestor;
			if (!last)
				last = 
				bin_tree<Key, Prior, Data>::head = 
				new typename bin_tree<Key, Prior, Data>::node(
					(*it).first,
					(*it).second,
					(*it).third,
					bin_tree<Key, Prior, Data>::head, nullptr);
			else {
				(*last).right = new 
					typename bin_tree<Key, Prior, Data>::node(
					(*it).first, (*it).second, (*it).third,
					(*last).right, nullptr, last);
				last = last->right;
			}
		}
	}
}

template <typename Key, typename Prior, typename Data>
void treap<Key, Prior, Data>::push(
	const utils::triple<Key, Prior, Data> &node) {
	auto res = bin_tree<Key, Prior, Data>::find(
		node.first,
		&(bin_tree<Key, Prior, Data>::head),
		nullptr,
		&(bin_tree<Key, Prior, Data>::head),
		[&node](const Prior &p) { return p <= node.second; }
	);
	auto p = bin_tree<Key, Prior, Data>::split(res.first, node.first);
	*(res.second.first) = new 
		typename bin_tree<Key, Prior, Data>::node(
			node.first, node.second, node.third,
			p.first, p.second, res.second.second);
}

template <typename Key, typename Prior, typename Data>
void treap<Key, Prior, Data>::pop(const Key &elem) {
	auto res = bin_tree<Key, Prior, Data>::find(
		elem,
		&(bin_tree<Key, Prior, Data>::head), 
		nullptr,
		&(bin_tree<Key, Prior, Data>::head));
	if (res.first) {
		auto cur = merge(
			(res.first)->left,
			(res.first)->right
		);
		*res.second.first = cur;
		if (cur) cur->ancestor = res.second.second;
		res.first->left = res.first->right = nullptr;
		delete res.first;
	}
}

template <typename Key, typename Prior, typename Data>
typename bin_tree<Key, Prior, Data>::node *
	treap<Key, Prior, Data>::merge(
		typename bin_tree<Key, Prior, Data>::node *f,
		typename bin_tree<Key, Prior, Data>::node *s) {
	if (!f || !s)
		return reinterpret_cast<typename bin_tree<Key, Prior, Data>::node *>((std::size_t)f + (std::size_t)s);
	if ((*f)->second < (*s)->second) {
		bin_tree<Key, Prior, Data>::bind<'r'>(f, merge(f->right, s));
		return f;
	}
	else {
		bin_tree<Key, Prior, Data>::bind<'l'>(s, merge(f, s->left));
		return s;
	}
}

template <typename Key, typename Prior, typename Data>
typename bin_tree<Key, Prior, Data>::node *
	treap<Key, Prior, Data>::cool_merge(
		typename bin_tree<Key, Prior, Data>::node *f,
		typename bin_tree<Key, Prior, Data>::node *s) {
	if (!f || !s)
		return reinterpret_cast<typename bin_tree<Key, Prior, Data>::node *>((std::size_t)f + (std::size_t)s);

	bool flag = (*f)->second < (*s)->second;
	typename bin_tree<Key, Prior, Data>::node 
		*max = flag ? f : s, *min = flag ? s : f;

	auto p = bin_tree<Key, Prior, Data>::split(min, (*max)->first);
	bin_tree<Key, Prior, Data>::bind<'l'>(max, cool_merge(max->left, p.first));
	bin_tree<Key, Prior, Data>::bind<'r'>(max, cool_merge(max->right, p.second));

	return max;
}
