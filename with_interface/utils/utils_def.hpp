#pragma once

#include <iostream>
#include <vector>
#include <sstream>
#include <iterator>
#include <fstream>

namespace utils {
	template <typename First, typename Second, typename Third>
	struct triple {
		First first;
		Second second;
		Third third;
		triple() = default;
		triple(const First &f,
			const Second &s, const Third &t) : 
			first(f), second(s), third(t) {}
	};

	template <typename First, typename Second, typename Third>
	triple<First, Second, Third> make_triple(const First &f,
		const Second &s, const Third &t) {
		return triple<First, Second, Third>(f, s, t);
	}

	struct cool_int {
		long long x;
		cool_int() = default;
		cool_int(long long x) : x(x) {}
		operator long long() {
			return x;
		}
		bool operator<=(const cool_int &oth) const {
			return x <= oth.x;
		}
		bool operator<(const cool_int &oth) const {
			return x < oth.x;
		}
	};
}

std::ostream &operator<<(std::ostream &out, utils::cool_int in) {
	int sign = 1 - 2 * (in.x < 0);
	in.x *= sign;
	std::stringstream ss("");
	for (; in.x >= 1000;) {
		for (int i = 0; i < 3; ++i, in.x /= 10)
			ss << in.x - (in.x / 10) * 10;
		ss << '.';
	}
	for (int i = 0; i < 3 && in.x; ++i, in.x /= 10)
		ss << in.x - (in.x / 10) * 10;
	if (!(sign + 1))
		ss << "-";
	std::string s = ss.str();
	copy(s.rbegin(), s.rend(), std::ostream_iterator<char>(out));
	return out;
}

template <typename T>
std::ostream &operator<<(std::ostream &out, const std::vector<T> &v) {
	out << *v.begin();
	for (auto it = ++v.begin(); it != v.end(); ++it)
		out << "\t" << *it;

	return out;
}
