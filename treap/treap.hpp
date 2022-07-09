#pragma once

#include <iostream>
#include <vector>
#include <clocale>
#include <string>
#include <algorithm>

#include "../utils.hpp"


template <typename Key, typename Prior>
class treap {
	template <typename Key1, typename Prior1>
	friend std::ostream &operator<<(std::ostream &out,
		const treap<Key1, Prior1> &tr);
public:
	treap() {}
	treap(std::vector<std::pair<Key, Prior>> &pairs) {
		std::sort(pairs.begin(), pairs.end(), [](decltype(**head) f, decltype(**head) s) {
			return f.first < s.first; });
		if (pairs.size()) {
			head = new node(pairs[0].first, pairs[0].second);
			node *last = head;
			auto it = pairs.begin();
			for (++it; it != pairs.end(); ++it) {
				while (last && (*last)->second > (*it).second)
					last = last->ancestor;
				if (!last)
					last = head = new node((*it).first, (*it).second, head, nullptr);
				else {
					(*last).right = new node((*it).first, (*it).second,
						(*last).right, nullptr, last);
					last = last->right;
				}
			}
		}
	}
	~treap() {
		delete head;
	}
	void push(const std::pair<Key, Prior> &pair) {
		node **last_b = &head, *last = nullptr, *cur = head;
		while (cur && (*cur)->second <= pair.second) {
			last = cur;
			if ((*cur)->first > pair.first) {
				last_b = &(cur->left);
				cur = cur->left;
			}
			else {
				last_b = &(cur->right);
				cur = cur->right;
			}
		}
		auto p = split(cur, pair.first);
		*last_b = new node(pair.first, pair.second, p.first, p.second, last);
	}
	void pop(const Key &elem) {
		node **last_b = &head, *last = nullptr, *cur = head;
		while (cur && (*cur)->first != elem) {
			last = cur;
			if ((*cur)->first > elem) {
				last_b = &(cur->left);
				cur = cur->left;
			}
			else {
				last_b = &(cur->right);
				cur = cur->right;
			}
		}
		if (cur) {
			*last_b = merge(cur->left, cur->right);
			if (*last_b) (*last_b)->ancestor = last;
			cur->left = cur->right = nullptr;
			delete cur;
		}
	}

	void join(treap &oth) { head = cool_merge(head, oth.head); }
private:
	class node {
	public:
		node *left, *right, *ancestor;
	private:
		std::pair<Key, Prior> kern;
	public:
		node(const Key &x, const Prior &y) : kern(std::make_pair(x, y)),
			left(nullptr), right(nullptr) {}
		node(const Key &x, const Prior &y,
			node *left, node *right) :
			kern(std::make_pair(x, y)), 
			left(left), right(right), ancestor(nullptr) {}
		node(const Key &x, const Prior &y,
			node *left, node *right, node *ancestor) :
			kern(std::make_pair(x, y)),
			left(left), right(right), ancestor(ancestor) {}
		~node();

		const decltype(kern) *operator->() const { return &kern; }
		const decltype(kern) &operator*() const { return kern; }

		void print(std::ostream &out) {
			out << "(" << kern.first << ", " << kern.second << ")";
		}
	};

	node *head = nullptr;

	void display(std::ostream &out, node *it,
		bool isr, std::string &&fmt) const {
		if (!it)
			return;
		out << fmt;
		out << (isr ? char(134) : char(132)) << " ";
		it->print(out);
		out << std::endl;
		if (it->left || it->right) {
			fmt.push_back((isr ? char(129) : ' '));
			fmt.push_back(' ');
			display(out, it->right, it->left, std::move(fmt));
			display(out, it->left, false, std::move(fmt));
			fmt.pop_back();
			fmt.pop_back();
		}
	}

	inline static const std::pair<node *, node *> split(node *v, Key c) {
		if (!v)
			return std::make_pair(nullptr, nullptr);
		if ((*v)->first > c) {
			auto p(split(v->left, c));
			bind<'l'>(v, p.second);
			return std::make_pair(p.first, v);
		}
		else {
			auto p(split(v->right, c));
			bind<'r'>(v, p.first);
			return std::make_pair(v, p.second);
		}
	}

	inline static node *merge(node *f, node *s) {
		if (!f || !s)
			return reinterpret_cast<node *>((std::size_t)f + (std::size_t)s);
		if ((*f)->second < (*s)->second) {
			bind<'r'>(f, merge(f->right, s));
			return f;
		}
		else {
			bind<'l'>(s, merge(f, s->left));
			return s;
		}
	}

	inline static node *cool_merge(node *f, node *s) {
		if (!f || !s)
			return reinterpret_cast<node *>((std::size_t)f + (std::size_t)s);

		bool flag = (*f)->second < (*s)->second;
		node *max = flag ? f : s, *min = flag ? s : f;

		auto p = split(min, (*max)->first);
		bind<'l'>(max, cool_merge(max->left, p.first));
		bind<'r'>(max, cool_merge(max->right, p.second));

		return max;
	}

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

//special for Gray *>*
template <typename Key, typename Prior>
treap<Key, Prior>::node::~node() {
	delete left;
	delete right;
}
