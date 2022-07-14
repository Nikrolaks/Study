#include <iostream>
#include <algorithm>
#include <vector>

//#include "utils/bin_tree/bin_tree.hpp"
#include "treap/treap.hpp"

using namespace std;


int main() {
	ios_base::sync_with_stdio(false);
	cin.tie(0);
	cout.tie(0);
	simple_treap<int> t({ -1, 2, -3, 4, -5, 6, -7, 8, -9 });
	cout << t << endl;
	t.push(12);
	cout << t << endl;
	t.pop(4);
	cout << t << endl;
	cout << t[-3] << endl;
}