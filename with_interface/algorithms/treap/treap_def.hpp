#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <random>

#include "utils.hpp"


template <typename Key, typename Prior>
class treap : public bin_tree<Key> {
protected:
	class node : public bin_tree<Key>::node {
	private:
		Prior p;
	public:
		node(const Key &key) : bin_tree<Key>::node(key) {}
		node(const Key &key, const Prior &p) :
			bin_tree<Key>::node(key), p(p) {}
		node(const std::pair<Key, Prior> &v) :
			bin_tree<Key>::node(v.first), p(v.second) {}

		node(const Key &key, const Prior &p,
			node *ancestor) :
			bin_tree<Key>::node(key, ancestor), p(p) {}

		node(const Key &key, const Prior &p, 
			node *left, node *right) :
			bin_tree<Key>::node(key, left, right), p(p) {}

		node(const Key &key, const Prior &p,
			node *left, node *right, node *ancestor) :
			bin_tree<Key>::node(key, left, right, ancestor), p(p) {}

		Prior &operator*() { return p; }

		virtual void print(std::ostream &out) const override {
			bin_tree<Key>::node::print(out);
			out << "(" << p << ")";
		}
	};

	treap(std::vector<node *> &nodes) {
		std::sort(nodes.begin(), nodes.end(),
			[](
				typename bin_tree<Key>::node *f,
				typename bin_tree<Key>::node *s) {
			return **f < **s;
		});
		build_treap(nodes);
	}

	void insert(node *v);

public:
	treap() {}
	treap(std::vector<std::pair<Key, Prior>> &&nodes);
	void push(const std::pair<Key, Prior> &unit);
	void pop(const Key &key) override;

	void join(treap &oth) { 
		this->head = cool_merge(this->head,	oth.head);
	}

private:
	void build_treap(const std::vector<node *> &nodes);

	inline static node *merge(node *f, node *s);

	inline static node *cool_merge(node *f, node *s);
};


template <typename Key>
class multiset : public treap<Key, utils::cool_int> {
protected:
	class node : public treap<Key, utils::cool_int>::node {
	private:
		friend class multiset; // kostil'

		static utils::cool_int get_random() {
			static bool flag = false;
			static std::default_random_engine gen;
			if (!flag) {
				gen.seed(14264);
				flag = true;
			}
			return gen();
		}
	public:
		node() {}
		node(const Key &key) :
			treap<Key, utils::cool_int>::node(key, get_random()) {}

		virtual void print(std::ostream &out) const override {
#ifdef _DEBUG
			treap<Key, utils::cool_int>::node::print(out);
#else
			bin_tree<Key>::node::print(out);
#endif
		}
	};

	multiset(std::vector<node *> &nodes) {
		new (dynamic_cast<treap<Key, utils::cool_int> *>(this))
			treap<Key, utils::cool_int>(nodes);
	}

public:
	multiset() {}
	multiset(const std::vector<Key> &elems) {
		/*
		std::vector<typename treap<Key, utils::cool_int>::node *> nodes;
		nodes.resize(elems.size());
		auto it = elems.cbegin();
		std::generate(nodes.begin(), nodes.end(), [&]() {
			return new node(*it++);
		});
		new (dynamic_cast<treap<Key, utils::cool_int> *>(this))
			treap<Key, utils::cool_int>(nodes);
			*/
		std::vector<std::pair<Key, utils::cool_int>> nodes(elems.size());
		auto it = elems.begin();
		std::generate(nodes.begin(), nodes.end(), [&]() {
			return std::make_pair(*it++, node::get_random());
		});
		new (dynamic_cast<treap<Key, utils::cool_int> *>(this))
			treap<Key, utils::cool_int>(std::move(nodes));
	}

	void push(const Key &elem) override {
		this->insert(new node(elem));
	}
};

template <typename Key, typename Data>
class map : public multiset<Key> {
protected:
	class node : public multiset<Key>::node {
	private:
		Data data;
	public:
		node(const Key &key) : multiset<Key>::node(key) {}
		node(const Key &key, const Data &data) :
			multiset<Key>::node(key), data(data) {}
		node(const std::pair<Key, Data> &v) :
			multiset<Key>::node(v.first), data(v.second) {}

		Data &operator*() { return data; }

		virtual void print(std::ostream &out) const override {
			multiset<Key>::node::print(out);
			out << " = " << data;
		}
	};

public:
	map() {}
	map(const std::vector<std::pair<Key, Data>> &elems) {
		std::vector<node *> nodes;
		nodes.resize(elems.size());
		auto it = elems.cbegin();
		std::generate(nodes.begin(), nodes.end(), [&]() {
			return new node(*it++);
		});
		new (dynamic_cast<multiset<Key> *>(this)) 
			multiset<Key>(nodes);
	}

	void push(const Key &key, const Data &data) {
		this->insert(new node(key, data));
	}

	void push(const std::pair<Key, Data> &elem) {
		dynamic_cast<multiset<Key> *>(this)->insert(new node(elem));
	}

	Data &operator[](const Key &key) {
		auto ret = *this->find(key, &(this->head));
		if (ret) {
			return **reinterpret_cast<node *>(ret);
		}
		else {
			push(key, Data());
			return **reinterpret_cast<node *>(*this->find(key, &(this->head)));
		}
	}
};
