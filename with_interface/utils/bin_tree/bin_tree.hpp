#pragma once

#include <iostream>
#include <vector>
#include <clocale>
#include <string>
#include <algorithm>

#include "bin_tree_def.hpp"

template <typename Key, typename Additional, typename Data>
class bin_tree<Key, Additional, Data>::node {
public:
	node *left, *right, *ancestor;
private:
	utils::triple<Key, Additional, Data> kern;
public:
	node(const Key &x, const Additional &y, const Data &z) : 
		kern(utils::make_triple(x, y, z)),
		left(nullptr), right(nullptr), ancestor(nullptr) {}
	node(const Key &x, const Additional &y, const Data &z,
		node *ancestor) :
		kern(utils::make_triple(x, y, z)),
		left(nullptr), right(nullptr),
		ancestor(ancestor) {}
	node(const Key &x, const Additional &y, const Data &z,
		node *left, node *right) :
		kern(utils::make_triple(x, y, z)),
		left(left), right(right), ancestor(nullptr) {}
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

	void print(std::ostream &out) {
		out << "[" << kern.first << " (" << kern.second << ")] = " << 
			kern.third;
	}

	void print_emblem(std::ostream &out) {
		out << "[" << char(148) << "]";
	}
};

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

//
//
// find realization
//
//

template <typename Key, typename Additional, typename Data>
typename bin_tree<Key, Additional, Data>::node **
	bin_tree<Key, Additional, Data>::find(
	const Key &k, node **cur) const {
	while (*cur && do_find_step(k, *cur, cur));
	return cur;
}

template <typename Key, typename Additional, typename Data>
const std::pair<
	typename bin_tree<Key, Additional, Data>::node **,
	bool> &
		bin_tree<Key, Additional, Data>::find(
		const Key &k, node **cur, bool side) const {
	while (*cur && do_find_step(k, *cur, side, cur));
	return std::move(std::make_pair(cur, side));
}

template <typename Key, typename Additional, typename Data>
const std::pair<
	typename bin_tree<Key, Additional, Data>::node *, 
	std::pair<
		typename bin_tree<Key, Additional, Data>::node **, 
		typename bin_tree<Key, Additional, Data>::node *>> &
			bin_tree<Key, Additional, Data>::find(
				const Key &k,
				node **last_b,
				node *last,
				node **cur) const {
	while (*cur && do_find_step(k, *cur, last_b, last, cur)) ;
	return std::move(std::make_pair(*cur, std::make_pair(last_b, last)));
}

//
// find with detecting throw additional data
//

template <typename Key, typename Additional, typename Data>
template <class TFunc>
typename bin_tree<Key, Additional, Data>::node **
	bin_tree<Key, Additional, Data>::find(
		const Key &k, node **cur, TFunc detect) const {
	while (*cur && detect((**cur)->second) && do_find_step(k, *cur, cur)) ;
	return cur;
}

template <typename Key, typename Additional, typename Data>
template <class TFunc>
const std::pair<
	typename bin_tree<Key, Additional, Data>::node **,
	bool> &
		bin_tree<Key, Additional, Data>::find(
			const Key &k,
			node **cur,
			bool side,
			TFunc detect) const {
	while (*cur && detect((**cur)->second) && do_find_step(k, *cur, side, cur)) ;
	return cur;
}

template <typename Key, typename Additional, typename Data>
template <class TFunc>
const std::pair<
	typename bin_tree<Key, Additional, Data>::node *,
	std::pair<
		typename bin_tree<Key, Additional, Data>::node **,
		typename bin_tree<Key, Additional, Data>::node *>> &
			bin_tree<Key, Additional, Data>::find(
				const Key &k, node **last_b,
				node *last, node **cur,
				TFunc detect) const {
	while (*cur && detect((**cur)->second) &&
		do_find_step(k, *cur, last_b, last, cur)) ;
	return std::move(std::make_pair(*cur, std::make_pair(last_b, last)));
}

//
//
// end of find realization
//
//

template <typename Key, typename Additional, typename Data>
void bin_tree<Key, Additional, Data>::push(
	const utils::triple<Key, Additional, Data> &node) {
	auto ppos = find(node.first, &head, false);
	if (*(ppos.first))
		return;
	bind(
		find_ancestor(ppos.first, ppos.second),
		new bin_tree<Key, Additional, Data>::node(
			node.first, node.second, node.third),
		ppos.second);
}

template <typename Key, typename Additional, typename Data>
void bin_tree<Key, Additional, Data>::pop(const Key &elem) {
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
