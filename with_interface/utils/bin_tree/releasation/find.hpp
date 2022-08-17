#pragma once

#include "../bin_tree_def.hpp"
#include "node.hpp"

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
	while (*cur && do_find_step(k, *cur, last_b, last, cur));
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
	while (*cur && detect((**cur)->second) && do_find_step(k, *cur, cur));
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
	while (*cur && detect((**cur)->second) && do_find_step(k, *cur, side, cur));
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
		do_find_step(k, *cur, last_b, last, cur));
	return std::move(std::make_pair(*cur, std::make_pair(last_b, last)));
}