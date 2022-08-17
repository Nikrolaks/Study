#pragma once

#include <iostream>
#include <vector>
#include <algorithm>

#include "treap_def.hpp"

template <typename Key, typename Prior>
treap<Key, Prior>::treap(std::vector<std::pair<Key, Prior>> &&elems) {
	std::sort(elems.begin(), elems.end(), [](
		const decltype(elems[0]) &f,
		const decltype(elems[0]) &s) {
		return f.first < s.first; 
	});
	std::vector<node *> nodes(elems.size());
	auto it = elems.begin();
	std::generate(nodes.begin(), nodes.end(), [&]() {
		return new node(*it++);
	});
	build_treap(nodes);
}

template <typename Key, typename Prior>
void treap<Key, Prior>::build_treap(const std::vector<node *> &nodes) {
	if (nodes.size()) {
		node *last;
		this->head = last = nodes[0];
		auto it = nodes.begin();
		for (++it; it != nodes.end(); last = *it, ++it) {
			while (last && (**last) > (***it))
				last = reinterpret_cast<node *>(last->ancestor);
			if (!last) {
				this->bind<'l'>(*it, this->head);
				this->head = *it;
			}
			else {
				this->bind<'l'>(*it, last->right);
				this->bind<'r'>(last, *it);
			}
		}
	}
}

template <typename Key, typename Prior>
void treap<Key, Prior>::push(const std::pair<Key, Prior> &unit) {
	auto res = this->find(
		unit.first,
		&(this->head),
		nullptr,
		&(this->head),
		[&unit](typename bin_tree<Key>::node *v) { 
			return **reinterpret_cast<node *>(v) <= unit.second;
		}
	);
	auto p = this->split(res.first, unit.first);
	*(res.second.first) = new node(unit.first, unit.second,
			p.first, p.second, res.second.second);
}

template <typename Key, typename Prior>
void treap<Key, Prior>::insert(node *v) {
	auto res = this->find(
		**((typename bin_tree<Key>::node *)(v)),
		&(this->head),
		false,
		[&v](typename bin_tree<Key>::node &w) {
			return *reinterpret_cast<node &>(w) <= **v;
		}
	);
	auto w = *(res.first);
	auto p = this->split(w,
		**((typename bin_tree<Key>::node *)(v)));
	this->bind<'l'>(v, p.first);
	this->bind<'r'>(v, p.second);
	this->bind(this->find_ancestor(res.first, res.second), v, res.second);
}

template <typename Key, typename Prior>
void treap<Key, Prior>::pop(const Key &elem) {
	auto res = this->find(
		elem,
		&(this->head), 
		nullptr,
		&(this->head));
	if (res.first) {
		auto cur = merge(
			reinterpret_cast<node *>((res.first)->left),
			reinterpret_cast<node *>((res.first)->right)
		);
		*res.second.first = cur;
		if (cur) cur->ancestor = res.second.second;
		res.first->left = res.first->right = nullptr;
		delete res.first;
	}
}

template <typename Key, typename Prior>
typename treap<Key, Prior>::node *
	treap<Key, Prior>::merge(node *f, node *s) {
	if (!f || !s)
		return reinterpret_cast<node *>((std::size_t)f + (std::size_t)s);
	if (**f < **s) {
		bin_tree<Key>::bind<'r'>(f, merge(
			reinterpret_cast<node *>(f->right),
			s)
		);
		return f;
	}
	else {
		bin_tree<Key>::bind<'l'>(s, merge(
			f,
			reinterpret_cast<node *>(s->left))
		);
		return s;
	}
}

template <typename Key, typename Prior>
typename treap<Key, Prior>::node *
	treap<Key, Prior>::cool_merge(node *f, node *s) {
	if (!f || !s)
		return reinterpret_cast<node *>((std::size_t)f + (std::size_t)s);

	bool flag = **f < **s;
	node *max = flag ? f : s, *min = flag ? s : f;

	auto p = this->split(min, *(bin_tree<Key> *(max)));
	this->bind<'l'>(max, cool_merge(max->left, p.first));
	this->bind<'r'>(max, cool_merge(max->right, p.second));

	return max;
}
