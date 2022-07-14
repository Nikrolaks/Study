#pragma once

#include <iostream>
#include <vector>
#include <clocale>
#include <string>
#include <random>

#include "../utils/utils.hpp"


template <typename Key, typename Prior, typename Data>
class treap : public bin_tree<Key, Prior, Data> {
public:
	treap() = default;
	treap(std::vector<utils::triple<Key, Prior, Data>> &nodes);
	void push(const utils::triple<Key, Prior, Data> &node) override;
	void pop(const Key &elem) override;

	void join(treap &oth) { 
		bin_tree<Key, Prior, Data>::head = cool_merge(
				bin_tree<Key, Prior, Data>::head,
				oth.head);
	}
private:
	inline static typename bin_tree<Key, Prior, Data>::node *merge(
		typename bin_tree<Key, Prior, Data>::node *f,
		typename bin_tree<Key, Prior, Data>::node *s);

	inline static typename bin_tree<Key, Prior, Data>::node *cool_merge(
		typename bin_tree<Key, Prior, Data>::node *f,
		typename bin_tree<Key, Prior, Data>::node *s);
};


template <typename Key, typename Data>
class int_treap : public treap<Key, decltype(std::random_device()()), Data> {
public:
	int_treap() = default;
	int_treap(const std::vector<std::pair<Key, Data>> &vec) {
		std::vector<utils::triple<Key, decltype(gen()), Data>> nodes;
		nodes.resize(vec.size());
		auto it = vec.cbegin();
		std::generate(nodes.begin(), nodes.end(), [&]() {
			auto to_ret = utils::make_triple((*it).first, gen(), (*it).second);
			it++;
			return to_ret;
		});
		new (dynamic_cast<treap<Key, decltype(std::random_device()()), Data> *>(this)) 
			treap<Key, decltype(std::random_device()()), Data>(nodes);
	}

	void push(const Key &elem, const Data &d) {
		dynamic_cast<
			treap<Key, 
			decltype(std::random_device()()),
			Data> *>(this)->push(
				utils::make_triple(elem, gen(), d));
	}
private:
	std::random_device gen;
};

template <typename Key>
class simple_treap : public int_treap<Key, bool> {
public:
	simple_treap() = default;
	simple_treap(const std::vector<Key> &vec) {
		std::vector<std::pair<Key, bool>> nodes;
		nodes.resize(vec.size());
		auto it = vec.cbegin();
		std::generate(nodes.begin(), nodes.end(), [&]() {
			return std::make_pair(*it++, true);
		});
		new (dynamic_cast<int_treap<Key, bool> *>(this)) 
			int_treap<Key, bool>(nodes);
	}

	void push(const Key &elem) {
		dynamic_cast<
			int_treap<Key,
			bool> *>(this)->push(elem, true);
	}
};