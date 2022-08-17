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
 */
template<typename Key>
class bin_tree {
	template <typename Key1>
	friend std::ostream &operator<<(std::ostream &out, const bin_tree &tr);
public:
	bin_tree() {}
	bin_tree(std::vector<Key> &&nodes);
	~bin_tree() {
		delete head;
	}
	virtual void push(const Key &node);
	virtual void pop(const Key &elem);
protected:
	class node;

	node *head = nullptr;


	bin_tree(std::vector<node *> &nodes);
	void insert(node *v);

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
	node *build_tree(const std::vector<node *> &nodes,
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
		if (**cur > k) {
			convert_step_params<'l'>(cur, rest...);
			return true;
		}
		if (**cur < k) {
			convert_step_params<'r'>(cur, rest...);
			return true;
		}
		return false;
	}
public:
	void draw(std::ostream &out) const {
		if (head)
			display(out, head, char(134), std::string(" "), 0);
	}
};

template <typename Key>
std::ostream &operator<<(std::ostream &out, const bin_tree<Key> &tr) {
	setlocale(LC_ALL, "Russian_Russia.20866");
	// if (tr.head)
	//	 tr.display(out, tr.head, char(134), std::string(" "), 0);
	tr.draw(out);
	return out;
}
