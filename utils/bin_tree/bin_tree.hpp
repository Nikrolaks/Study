#pragma once

#include <iostream>
#include <vector>
#include <clocale>
#include <string>
#include <algorithm>

#include "bin_tree_def.hpp"

template <typename Key, typename NodeKern>
class bin_tree<Key, NodeKern>::node {
public:
	node *left, *right, *ancestor;
private:
	std::pair<Key, NodeKern> kern;
public:
	node(const Key &x, const NodeKern &y) : kern(std::make_pair(x, y)),
		left(nullptr), right(nullptr), ancestor(nullptr) {}
	node(const Key &x, const NodeKern &y, node *ancestor) :
		kern(std::make_pair(x, y)),
		left(nullptr), right(nullptr),
		ancestor(ancestor) {}
	node(const Key &x, const NodeKern &y,
		node *left, node *right) :
		kern(std::make_pair(x, y)),
		left(left), right(right), ancestor(nullptr) {}
	node(const Key &x, const NodeKern &y,
		node *left, node *right, node *ancestor) :
		kern(std::make_pair(x, y)),
		left(left), right(right), ancestor(ancestor) {}
	~node() {
		delete left;
		delete right;
	}

	const decltype(kern) *operator->() const { return &kern; }
	const decltype(kern) &operator*() const { return kern; }

	void print(std::ostream &out) {
		out << "(" << kern.first << ", " << kern.second << ")";
	}
};

template <typename Key, typename NodeKern>
typename bin_tree<Key, NodeKern>::node *bin_tree<Key, NodeKern>::build_tree(
	const std::vector<std::pair<Key, NodeKern>> &pairs,
	std::size_t left, std::size_t right) {
	if (left >= right) {
		return nullptr;
	}
	std::size_t mid = (left + right) >> 1;
	node *res = new node(pairs[mid].first, pairs[mid].second);
	bind<'l'>(res, build_tree(pairs, left, mid));
	bind<'r'>(res, build_tree(pairs, mid + 1, right));
	return res;
}

template <typename Key, typename NodeKern>
bin_tree<Key, NodeKern>::bin_tree(std::vector<std::pair<Key, NodeKern>> &pairs) {
	std::sort(pairs.begin(), pairs.end(), [](decltype(**head) f, decltype(**head) s) {
		return f.first < s.first; });
	head = build_tree(pairs, 0, pairs.size());
}

template <typename Key, typename NodeKern>
typename bin_tree<Key, NodeKern>::node **bin_tree<Key, NodeKern>::find(
	const Key &k, node **cur) const {
	if (*cur) {
		if ((**cur)->first > k)
			return find(k, &((*cur)->left));
		else if ((**cur)->first < k)
			return find(k, &((*cur)->right));
	}
	return cur;
}

template <typename Key, typename NodeKern>
const std::pair<typename bin_tree<Key, NodeKern>::node **, bool> &
	bin_tree<Key, NodeKern>::find(
		const Key &k, node **cur, bool side) const {
	if (*cur) {
		if ((**cur)->first > k)
			return find(k, &((*cur)->left), false);
		if ((**cur)->first < k)
			return find(k, &((*cur)->right), true);
	}
	return std::move(std::make_pair(cur, side));
}

template <typename Key, typename NodeKern>
void bin_tree<Key, NodeKern>::push(const std::pair<Key, NodeKern> &pair) {
	auto ppos = find(pair.first, &head, false);
	if (*(ppos.first))
		return;
	bind(
		find_ancestor(ppos.first, ppos.second),
		new node(pair.first, pair.second),
		ppos.second);
}

template <typename Key, typename NodeKern>
void bin_tree<Key, NodeKern>::pop(const Key &elem) {
	auto pret = find(elem, &head, false);
	if (*(pret.first)) {
		node **pos = find(elem, &((*(pret.first))->left));
		node *obj = *pret.first;
		if (pos == &(obj->left)) { // empty left tree
			if (pret.first != &head)
				bind(find_ancestor(pret.first, pret.second),
					obj->right, pret.second);
			else
				head = obj->right;
		}
		else if (pos == &(obj->left->right)) { // left tree is tree without
											   // first right branch
			if (pret.first != &head)
				bind(find_ancestor(pret.first, pret.second),
					obj->left, pret.second);
			else
				head = obj->left;

			obj->left->right = obj->right;
		}
		else { // full left tree
			if (pret.first != &head)
				bind(find_ancestor(pret.first, pret.second),
					find_ancestor<'r'>(pos), pret.second);
			else
				head = find_ancestor<'r'>(pos);
			bind<'l'>(*(pret.first), obj->left);
			bind<'r'>(*(pret.first), obj->right);
		}
		head->ancestor = nullptr;
		obj->left = nullptr;
		obj->right = nullptr;
		delete obj;
	}
}

template <typename Key, typename NodeKern>
void bin_tree<Key, NodeKern>::display(std::ostream &out, node *it,
	bool isr, std::string &&fmt) const {
	if (!it)
		return;
	out << fmt;
	out << (isr ? char(134) : char(132)) << " ";
	it->print(out);
	out << std::endl;
	if (it->left || it->right) {
		fmt.push_back((isr ? char(129) : ' '));
		fmt.push_back(' ');
		display(out, it->right, it->left, std::move(fmt));
		display(out, it->left, false, std::move(fmt));
		fmt.pop_back();
		fmt.pop_back();
	}
}

template <typename Key, typename NodeKern>
const std::pair<typename bin_tree<Key, NodeKern>::node *,
	typename bin_tree<Key, NodeKern>::node *>
	bin_tree<Key, NodeKern>::split(
		typename bin_tree<Key, NodeKern>::node *v, Key c) {
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