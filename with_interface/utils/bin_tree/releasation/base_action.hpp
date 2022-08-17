#pragma once

#include "../bin_tree_def.hpp"
#include "node.hpp"

template <typename Key>
void bin_tree<Key>::push(const Key &unit) {
	insert(new node(unit));
}

template <typename Key>
void bin_tree<Key>::pop(const Key &elem) {
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
