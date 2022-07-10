#include <iostream>
#include <vector>
#include <random>

#include "utils.hpp"
#include "sorts/sorts_n2.hpp"
#include "sorts/sorts_nlog_n.hpp"
#include "treap/treap.hpp"

using namespace std;


int main() {
	int_treap<int>
		t({1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14}),
		r({ -1, -2, -3, 114, -5, -6, 17, -8, -9, -10, 111, -12, -13, -14 });
	cout << t << endl << r << endl;
	t.join(r);
	cout << t << endl;
	t.push(1212);
	cout << t << endl;
}