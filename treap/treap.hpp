#pragma once

#include <iostream>
#include <vector>
#include <clocale>
#include <string>
#include <algorithm>

#include "treap_def.hpp"

template <typename Key, typename Prior>
treap<Key, Prior>::treap(std::vector<std::pair<Key, Prior>> &pairs) {
	std::sort(pairs.begin(), pairs.end(), [](
		decltype(**bin_tree<Key, Prior>::head) f,
		decltype(**bin_tree<Key, Prior>::head) s) {
		return f.first < s.first; });
	if (pairs.size()) {
		bin_tree<Key, Prior>::head = 
			new typename bin_tree<Key, Prior>::node(
				pairs[0].first, pairs[0].second);
		typename bin_tree<Key, Prior>::node *last = 
			bin_tree<Key, Prior>::head;
		auto it = pairs.begin();
		for (++it; it != pairs.end(); ++it) {
			while (last && (*last)->second > (*it).second)
				last = last->ancestor;
			if (!last)
				last = 
				bin_tree<Key, Prior>::head = 
				new typename bin_tree<Key, Prior>::node(
					(*it).first,
					(*it).second,
					bin_tree<Key, Prior>::head, nullptr);
			else {
				(*last).right = new 
					typename bin_tree<Key, Prior>::node(
					(*it).first, (*it).second,
					(*last).right, nullptr, last);
				last = last->right;
			}
		}
	}
}

template <typename Key, typename Prior>
void treap<Key, Prior>::push(const std::pair<Key, Prior> &pair) {
	typename bin_tree<Key, Prior>::node **last_b = 
		&(bin_tree<Key, Prior>::head),
		*last = nullptr,
		*cur = bin_tree<Key, Prior>::head;
	while (cur && (*cur)->second <= pair.second) {
		last = cur;
		if ((*cur)->first > pair.first) {
			last_b = &(cur->left);
			cur = cur->left;
		}
		else {
			last_b = &(cur->right);
			cur = cur->right;
		}
	}
	auto p = bin_tree<Key, Prior>::split(cur, pair.first);
	*last_b = new 
		typename bin_tree<Key, Prior>::node(
			pair.first, pair.second, p.first, p.second, last);
}

template <typename Key, typename Prior>
void treap<Key, Prior>::pop(const Key &elem) {
	auto res = bin_tree<Key, Prior>::find(
		elem,
		&(bin_tree<Key, Prior>::head), 
		nullptr,
		bin_tree<Key, Prior>::head);
	if (res.first) {
		auto cur = 
			*res.second.first = merge(res.first->left, res.first->right);
		if (cur) cur->ancestor = res.second.second;
		res.first->left = res.first->right = nullptr;
		delete res.first;
	}
}

template <typename Key, typename Prior>
typename bin_tree<Key, Prior>::node *
	treap<Key, Prior>::merge(
		typename bin_tree<Key, Prior>::node *f,
		typename bin_tree<Key, Prior>::node *s) {
	if (!f || !s)
		return reinterpret_cast<typename bin_tree<Key, Prior>::node *>((std::size_t)f + (std::size_t)s);
	if ((*f)->second < (*s)->second) {
		bin_tree<Key, Prior>::bind<'r'>(f, merge(f->right, s));
		return f;
	}
	else {
		bin_tree<Key, Prior>::bind<'l'>(s, merge(f, s->left));
		return s;
	}
}

template <typename Key, typename Prior>
typename bin_tree<Key, Prior>::node *
	treap<Key, Prior>::cool_merge(
		typename bin_tree<Key, Prior>::node *f,
		typename bin_tree<Key, Prior>::node *s) {
	if (!f || !s)
		return reinterpret_cast<typename bin_tree<Key, Prior>::node *>((std::size_t)f + (std::size_t)s);

	bool flag = (*f)->second < (*s)->second;
	typename bin_tree<Key, Prior>::node 
		*max = flag ? f : s, *min = flag ? s : f;

	auto p = bin_tree<Key, Prior>::split(min, (*max)->first);
	bin_tree<Key, Prior>::bind<'l'>(max, cool_merge(max->left, p.first));
	bin_tree<Key, Prior>::bind<'r'>(max, cool_merge(max->right, p.second));

	return max;
}

