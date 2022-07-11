#pragma once

#include <iostream>
#include <vector>

#include "heap/heap.hpp"
#include "bin_tree/bin_tree.hpp"

namespace utils {
}

template <typename T>
std::ostream &operator<<(std::ostream &out, const std::vector<T> &v) {
	out << *v.begin();
	for (auto it = ++v.begin(); it != v.end(); ++it)
		out << "\t" << *it;

	return out;
}
