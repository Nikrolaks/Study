#pragma once

#include <iostream>
#include <vector>
#include <clocale>
#include <string>
#include <cstddef>


template<typename Key, typename NodeKern>
class bin_tree {
	template <typename Key1, typename Prior1>
	friend std::ostream &operator<<(std::ostream &out,
		const bin_tree<Key1, Prior1> &tr);
public:
	bin_tree() {}
	bin_tree(std::vector<std::pair<Key, NodeKern>> &nodes);
	~bin_tree() {
		delete head;
	}
	virtual void push(const std::pair<Key, NodeKern> &node);
	virtual void pop(const Key &elem);

	const NodeKern &operator[](const Key &k) { return (**find(k, &head))->second; }
protected:
	class node;

	node *head = nullptr;

	inline static const std::pair<node *, node *> split(node *v, Key c);

	const std::pair<node **, bool> &
		find(const Key &k, node **cur, bool side) const;
	node **find(const Key &k, node **cur) const;

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
	node *build_tree(const std::vector<std::pair<Key, NodeKern>> &pairs,
		std::size_t left, std::size_t right);
	void display(std::ostream &out, node *it,
		bool isr, std::string &&fmt) const;
};

template <typename Key, typename Prior>
std::ostream &operator<<(std::ostream &out, const bin_tree<Key, Prior> &tr) {
	setlocale(LC_ALL, "Russian_Russia.20866");
	if (tr.head)
		tr.display(out, tr.head, false, std::string(""));
	return out;
}
