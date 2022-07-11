#include <iostream>
#include <vector>
#include <random>

#include "utils/utils.hpp"
#include "sorts/sorts_n2.hpp"
#include "sorts/sorts_nlog_n.hpp"
#include "treap/treap.hpp"

using namespace std;


int main() {
	int_treap<int> t({ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 });
	cout << t << endl;
	t.push(-1);
	cout << t << endl;
	t.pop(8);
	cout << t << endl;
	int_treap<int> r({ 0, -2, -3, -4, 11, 12, 13 });
	cout << r << endl;
	t.join(r);
	cout << t << endl;
}