#pragma once

#include "../bin_tree_def.hpp"
#include "node.hpp"

template <typename Key>
typename bin_tree<Key>::node **
bin_tree<Key>::find(
	const Key &k, node **cur) const {
	while (*cur && do_find_step(k, *cur, cur));
	return cur;
}

template <typename Key>
const std::pair<typename bin_tree<Key>::node **, bool> &
	bin_tree<Key>::find(const Key &k, node **cur, bool side) const {
	while (*cur && do_find_step(k, *cur, side, cur));
	return std::move(std::make_pair(cur, side));
}

template <typename Key>
const std::pair<
		typename bin_tree<Key>::node *,
		std::pair<
			typename bin_tree<Key>::node **,
			typename bin_tree<Key>::node *
		>
	> &
	bin_tree<Key>::find(const Key &k, node **last_b,
		node *last, node **cur) const {
	while (*cur && do_find_step(k, *cur, last_b, last, cur));
	return std::move(std::make_pair(*cur, std::make_pair(last_b, last)));
}

//
// find with detecting throw node content
// Requirements:
//  > TFunc looks like:
//    bool detect(const node &v);
//

template <typename Key>
template <class TFunc>
typename bin_tree<Key>::node **
bin_tree<Key>::find(const Key &k, node **cur, TFunc detect) const {
	while (*cur && detect(**cur) && do_find_step(k, *cur, cur));
	return cur;
}

template <typename Key>
template <class TFunc>
const std::pair<typename bin_tree<Key>::node **, bool> &
	bin_tree<Key>::find(const Key &k, node **cur, bool side,
		TFunc detect) const {
	while (*cur && detect(**cur) && do_find_step(k, *cur, side, cur));
	return cur;
}

template <typename Key>
template <class TFunc>
const std::pair<
		typename bin_tree<Key>::node *,
		std::pair<
			typename bin_tree<Key>::node **,
			typename bin_tree<Key>::node *
		>
	> &bin_tree<Key>::find(const Key &k, node **last_b,
		node *last, node **cur,	TFunc detect) const {
	while (*cur && detect(**cur) &&	do_find_step(k, *cur, last_b, last, cur));
	return std::move(std::make_pair(*cur, std::make_pair(last_b, last)));
}
