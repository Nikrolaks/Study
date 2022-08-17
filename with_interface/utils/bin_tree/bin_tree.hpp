#pragma once

#include <iostream>
#include <string>
#include <algorithm>

#include "bin_tree_def.hpp"
#include "releasation/node.hpp"
#include "releasation/base_action.hpp"
#include "releasation/private_base_action.hpp"
#include "releasation/protected_base_action.hpp"
#include "releasation/find.hpp"


template <typename Key>
bin_tree<Key>::bin_tree(
	std::vector<Key> &&nodes) {
	std::sort(nodes.begin(), nodes.end());
	std::vector<node *> to_init(nodes.size());
	auto it = nodes.begin();
	std::generate(to_init.begin(), to_init.end(),
		[&it]() { return new node(*it++); });
	head = build_tree(to_init, 0, nodes.size());
}

template <typename Key>
bin_tree<Key>::bin_tree(std::vector<node *> &nodes) {
	std::sort(nodes.begin(), nodes.end(),
		[&](const decltype(nodes[0]) &f,
			const decltype(nodes[0]) &s) {
		return (*f)->key < (*s)->key;
	});
	head = build_tree(nodes, 0, nodes.size());
}

template <typename Key>
void bin_tree<Key>::display(std::ostream &out, node *it,
	char v_symb, std::string &&fmt, utils::cool_int deep) const {
	if (!it)
		return;
	if (it->right) {
		fmt.push_back((v_symb == char(132) ? char(129) : ' '));
		fmt.push_back(' ');
		display(out, it->right, char(130), std::move(fmt), deep + 1);
		fmt.pop_back();
		fmt.pop_back();
	}
	out << char(135) << deep.x << char(134) << fmt;
	out << v_symb << " ";
	it->print(out);
	out << std::endl;
	if (it->left) {
		fmt.push_back((v_symb == char(130) ? char(129) : ' '));
		fmt.push_back(' ');
		display(out, it->left, char(132), std::move(fmt), deep + 1);
		fmt.pop_back();
		fmt.pop_back();
	}
}

template<typename Key, typename Data>
class dictionary : public bin_tree<Key> {
protected:
	class d_node : public bin_tree<Key>::node {
	private:
		Data data;
	public:
		d_node(const Key &key) : bin_tree<Key>::node(key) {}
		d_node(const Key &key, const Data &data) :
			bin_tree<Key>::node(key), data(data) {}
		d_node(const std::pair<Key, Data> &v) :
			bin_tree<Key>::node(v.first), data(v.second) {}

		Data &operator*() { return data; }

		virtual void print(std::ostream &out) override {
			bin_tree<Key>::node::print(out);
			out << " = " << data;
		}
	};

public:
	dictionary() {}
	dictionary(const std::vector<std::pair<Key, Data>> &elems) {
		std::vector<d_node *> nodes(elems.size());
		auto it = elems.begin();
		std::generate(nodes.begin(), nodes.end(), [&it]() { return new d_node(*it++); });
		new (dynamic_cast<bin_tree<Key> *>(this)) bin_tree<Key>(nodes);
	}

	void push(const Key &key, const Data &data) {
		this->insert(new d_node(key, data));
	}

	// Requarements:
	// Data - default constructable
	Data &operator[](const Key &key) {
		auto ret = *this->find(key, &(this->head));
		if (ret) {
			return **reinterpret_cast<d_node *>(ret);
		}
		else {
			push(key, Data());
			return **reinterpret_cast<d_node *>(*this->find(key, &(this->head)));
		}
	}
};
