#include <iostream>
#include <vector>
#include <random>

#include "utils.hpp"
#include "sorts/sorts_n2.hpp"
#include "sorts/sorts_nlog_n.hpp"
#include "treap/treap.hpp"

using namespace std;


int main() {
	std::random_device gen;
	
	std::vector<std::pair<int, decltype(gen())>> v = {
		std::make_pair(1, 1),
		std::make_pair(2, 100500),
		std::make_pair(3, 7),
	};
	treap<int, decltype(gen())> t(v), r;
	r.push(std::make_pair(0, 12));
	r.push(std::make_pair(-4, -13));
	r.push(std::make_pair(5, 120));
	r.push(std::make_pair(-6, 1111));
	cout << "Before join:\n" << t << endl << r << endl;
	t.join(r);
	cout << "After join:\n" << t << endl;
}