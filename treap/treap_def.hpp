#pragma once

#include <iostream>
#include <vector>
#include <clocale>
#include <string>
#include <random>

#include "../utils/utils.hpp"


template <typename Key, typename Prior>
class treap : public bin_tree<Key, Prior> {
public:
	treap() {}
	treap(std::vector<std::pair<Key, Prior>> &pairs);
	void push(const std::pair<Key, Prior> &pair) override;
	void pop(const Key &elem) override;

	void join(treap &oth) { 
		bin_tree<Key, Prior>::head = cool_merge(
				bin_tree<Key, Prior>::head,
				oth.head);
	}
private:
	inline static typename bin_tree<Key, Prior>::node *merge(
		typename bin_tree<Key, Prior>::node *f,
		typename bin_tree<Key, Prior>::node *s);

	inline static typename bin_tree<Key, Prior>::node *cool_merge(
		typename bin_tree<Key, Prior>::node *f,
		typename bin_tree<Key, Prior>::node *s);
};


template <typename Key>
class int_treap : public treap<Key, decltype(std::random_device()())> {
public:
	int_treap() {}
	int_treap(const std::vector<Key> &vec) {
		std::vector<std::pair<Key, decltype(gen())>> pairs;
		pairs.resize(vec.size());
		auto it = vec.cbegin();
		std::generate(pairs.begin(), pairs.end(), [&]() {
			return std::make_pair(*it++, gen());
		});
		// Say thanks to compiler that
		// can't see parent with instantiated template
		// constructor and methods 
		new (dynamic_cast<treap<Key, decltype(std::random_device()())> *>(this)) 
			treap<Key, decltype(std::random_device()())>(pairs);
	}

	void push(const Key &elem) {
		dynamic_cast<
			treap<Key, 
			decltype(std::random_device()())> *>(this)->push(
				std::make_pair(elem, gen()));
	}
private:
	std::random_device gen;
};
