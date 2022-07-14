#pragma once

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
}