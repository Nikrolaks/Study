#include <iostream>
#include <algorithm>
#include <vector>

#include "utils/bin_tree/bin_tree.hpp"
#include "treap/treap.hpp"

using namespace std;

class foo {
public:
	foo() : r(4) {
	}

	void do_actions() {
		r[2].a = 1;
		cout << r[2].a << endl;
	}


private:
	struct add {
		int a;
		double rest;
	};

	vector<add> r;
};

int main() {
	ios_base::sync_with_stdio(false);
	cin.tie(0);
	cout.tie(0);
	foo d;
	d.do_actions();
}