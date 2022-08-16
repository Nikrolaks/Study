#pragma once

#include <iostream>
#include <vector>
#include <algorithm>
#include <clocale>
#include <string>
#include <cstddef>

#include "utils_def.hpp"

/*
 ~ Binary tree ~
 Requirements:
	> Key - comparable
	> Additional - copyable
	> Data - copyable
 */
template<typename Key, typename Additional, typename Data>
class bin_tree {
	template <typename Key1, typename Additional1, typename Data1>
	friend std::ostream &operator<<(std::ostream &out,
		const bin_tree<Key1, Additional1, Data1> &tr);
public:
	bin_tree() {}
	bin_tree(std::vector<utils::triple<Key, Additional, Data>> &nodes);
	~bin_tree() {
		delete head;
	}
	virtual void push(const utils::triple<Key, Additional, Data> &node);
	// Requirements:
	//    > Additional - default constructable
	virtual void push(const std::pair<Key, Data> &node) {
		push(utils::make_triple(node.first, Additional(), node.second));
	}
	virtual void pop(const Key &elem);

	// Requirements:
	//    > Data - default constructable
	virtual Data &operator[](const Key &k) {
		auto r = *find(k, &head);
		if (r)
			return (*r)->third;
		else {
			push(std::make_pair(k, Data()));
			return (**find(k, &head))->third;
		}
	}
protected:
	class node;

	node *head = nullptr;

	inline static const std::pair<node *, node *> split(node *v, Key c);

	//
	// find overloads
	//

	node **find(const Key &k, node **cur) const;
	const std::pair<node **, bool> &
		find(const Key &k, node **cur, bool side) const;
	const std::pair<node *, std::pair<node **, node *>> &
		find(const Key &k, node **last_b, node *last, node **cur) const;

	template <class TFunc>
	node **find(const Key &k, node **cur, TFunc detect) const;

	template <class TFunc>
	const std::pair<node **, bool> &
		find(const Key &k, node **cur, bool side, TFunc detect) const;

	template <class TFunc>
	const std::pair<node *, std::pair<node **, node *>> &
		find(const Key &k, node **last_b, node *last,
			node **cur, TFunc detect) const;


	template <char c> inline static void bind(node *anc, node *child) {}
	template <> inline static void bind<'l'>(node *anc, node *child) {
		anc->left = child;
		if (child) child->ancestor = anc;
	}
	template <> inline static void bind<'r'>(node *anc, node *child) {
		anc->right = child;
		if (child) child->ancestor = anc;
	}
	inline static void bind(node *anc, node *child, bool side) {
		if (side)
			bind<'r'>(anc, child);
		else
			bind<'l'>(anc, child);
	}

	template <char c> inline static node *find_ancestor(node **child) {}
	template <> inline static node *find_ancestor<'l'>(node **child) {
		return (node *)((std::size_t)(child) - offsetof(node, left));
	}
	template <> inline static node *find_ancestor<'r'>(node **child) {
		return (node *)((std::size_t)(child) - offsetof(node, right));
	}
	inline static node *find_ancestor(node **child, bool side) {
		if (side)
			return find_ancestor<'r'>(child);
		else
			return find_ancestor<'l'>(child);
	}
private:
	node *build_tree(
		const std::vector<
		utils::triple<Key, Additional, Data>> &nodes,
		std::size_t left, std::size_t right);
	void display(std::ostream &out, node *it,
		char v_symb, std::string &&fmt,
		utils::cool_int deep) const;

	//
	//
	// convert_step_params realization
	//
	//


	// in-time
	template <char c> void convert_step_params(node *cur, bool &side) const {}
	template <char c> void convert_step_params(
		node *cur, node **&last_b) const {} // it also recursion end
	template <char c> void convert_step_params(node *cur, node *&last) const {
		last = cur;
	} 
	
	// recursion template
	template <char c, typename First, typename... Rest> 
	void convert_step_params(node *cur, First &f, Rest &... rest) const {
		convert_step_params<c>(cur, f);
		convert_step_params<c>(cur, rest...);
	}

	//
	// start specialization
	//

	template <>
	void convert_step_params<'l'>(node *cur, bool &side) const {
		side = false;
	}

	template <>
	void convert_step_params<'r'>(node *cur, bool &side) const {
		side = true;
	}

	template <>
	void convert_step_params<'l'>(node *cur, node **&last_b) const {
		last_b = &(cur->left);
	}

	template <>
	void convert_step_params<'r'>(node *cur, node **&last_b) const {
		last_b = &(cur->right);
	}

	// lego is coming to end

	template <typename... Rest>
	bool do_find_step(const Key &k, node *cur, Rest &... rest) const {
		if ((*cur)->first > k) {
			convert_step_params<'l'>(cur, rest...);
			return true;
		}
		if ((*cur)->first < k) {
			convert_step_params<'r'>(cur, rest...);
			return true;
		}
		return false;
	}
};

template <typename Key, typename Additional, typename Data>
std::ostream &operator<<(std::ostream &out, const bin_tree<Key, Additional, Data> &tr) {
	setlocale(LC_ALL, "Russian_Russia.20866");
	if (tr.head)
		tr.display(out, tr.head, char(134), std::string(" "), 0);
	return out;
}

template<typename Key>
class simple_tree : public bin_tree<Key, bool, bool> {
public:
	simple_tree() {}
	simple_tree(const std::vector<Key> &elems) {
		std::vector<utils::triple<Key, bool, bool>> to_init(elems.size());
		auto it = elems.cbegin();
		std::generate(to_init.begin(), to_init.end(),
			[&it]() { return utils::make_triple(*it++, true, true); });
		new (dynamic_cast<bin_tree<Key, bool, bool> *>(this))
			bin_tree<Key, bool, bool>(to_init);
	}

	void push(const Key &elem) {
		dynamic_cast<
			bin_tree<Key, bool, bool> *>(this)->push(
				utils::make_triple(elem, true, true));
	}
};