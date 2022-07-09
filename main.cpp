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
		std::make_pair(1, gen()),
		std::make_pair(2, gen()),
		std::make_pair(3, gen()),
	};
	treap<int, decltype(gen())> t(v);
	//cout << "Before:\n" << t << std::endl;
	//t.push(std::make_pair(0, gen()));
	//cout << "Add 0:\n" << t << std::endl;
	//t.push(std::make_pair(4, gen()));
	//cout << "Add 4:\n" << t << std::endl;
	//t.push(std::make_pair(5, gen()));
	//cout << "Add 5:\n" << t << std::endl;
	//t.push(std::make_pair(6, gen()));
	//cout << "Add 6:\n" << t << std::endl;
	t.print();
}