#pragma once

#include <iostream>
#include <vector>
#include <clocale>
#include <string>
#include <algorithm>
#include <random>

#include "../utils.hpp"


template <typename Key, typename Prior>
class treap {
	template <typename Key1, typename Prior1>
	friend std::ostream &operator<<(std::ostream &out,
		const treap<Key1, Prior1> &tr);
public:
	treap() {}
	treap(std::vector<std::pair<Key, Prior>> &pairs);
	~treap() {
		delete head;
	}
	void push(const std::pair<Key, Prior> &pair);
	void pop(const Key &elem);

	void join(treap &oth) { head = cool_merge(head, oth.head); }
private:
	class node;

	node *head = nullptr;

	void display(std::ostream &out, node *it,
		bool isr, std::string &&fmt) const;

	inline static const std::pair<node *, node *> split(node *v, Key c);

	inline static node *merge(node *f, node *s);

	inline static node *cool_merge(node *f, node *s);

	template <char c> inline static void bind(node *anc, node *child) {};
	template <> inline static void bind<'l'>(node *anc, node *child) {
		anc->left = child;
		if (child) child->ancestor = anc;
	};
	template <> inline static void bind<'r'>(node *anc, node *child) {
		anc->right = child;
		if (child) child->ancestor = anc;
	};
};

template <typename Key, typename Prior>
std::ostream &operator<<(std::ostream &out, const treap<Key, Prior> &tr) {
	setlocale(LC_ALL, "Russian_Russia.20866");
	if (tr.head)
		tr.display(out, tr.head, false, std::string(""));
	return out;
}

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
		dynamic_cast<treap<Key, decltype(std::random_device()())> *>(this)->push(std::make_pair(elem, gen()));
	}
private:
	std::random_device gen;
};
