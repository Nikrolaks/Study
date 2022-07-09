#pragma once

#include <iostream>
#include <vector>

#include "heap_utils.hpp"

namespace utils {
}

template <typename T>
std::ostream &operator<<(std::ostream &out, const std::vector<T> &v) {
	out << *v.begin();
	for (auto it = ++v.begin(); it != v.end(); ++it)
		out << "\t" << *it;

	return out;
}
