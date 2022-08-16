#pragma once

#include <vector>
#include <algorithm>
#include <clocale>
#include <string>

template <typename T>
class heap {
public:
	heap() {}
	heap(const std::vector<T> &mas) : t(mas) {
		for (std::size_t i = mas.size() - 1; i > 0; --i)
			shift_down(i);
	}

	const T &get_head() const {
		return t[0];
	}

	void remove_head() {
		t[0] = *(t.rbegin());
		t.pop_back();
		if (!clear())
			shift_down(1);
	}

	bool clear() const {
		return t.size() == 0;
	}

	void display(std::ostream &out,
		std::size_t v, std::size_t deep, bool isr,
		std::string &fmt) const {
		if (v > t.size())
			return;
		if (deep) {
			fmt.push_back(!isr ? char(129) : ' ');
			fmt.push_back(' ');
		}
		display(out, v << 1, deep + 1, true, fmt);
		if (deep) {
			fmt.pop_back();
			fmt.pop_back();
		}
		out << fmt;
		if (deep)
			out << (isr ? char(130) : char(132)) << " ";
		out << t[v - 1] << std::endl;
		if (deep) {
			fmt.push_back((isr ? char(129) : ' '));
			fmt.push_back(' ');
		}
		display(out, (v << 1) + 1, deep + 1, false, fmt);
		if (deep) {
			fmt.pop_back();
			fmt.pop_back();
		}
	}
private:
	void shift_down(std::size_t v) {
		std::size_t dbl;
		for (dbl = v << 1;
			dbl < t.size() &&
				(t[v - 1] > t[dbl - 1] || t[v - 1] > t[dbl]);
			dbl = v << 1) {
			auto m = std::min(dbl - 1, dbl,
				[&](const decltype(dbl) &f,
					const decltype(dbl) &s) {
				return t[f] < t[s];
			});
			std::swap(t[v - 1], t[m]);
			v = m + 1;
		}
		if (dbl - 1 < t.size() && t[v - 1] > t[dbl - 1])
			std::swap(t[v - 1], t[dbl - 1]);
	}
	std::vector<T> t;
};

template <typename T>
std::ostream &operator<<(std::ostream &out, const heap<T> &h) {
	setlocale(LC_ALL, "Russian_Russia.20866");
	std::string fmt;
	h.display(out, 1, 0, false, fmt);
	return out;
}