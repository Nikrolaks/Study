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
	int_treap<int, std::string> ma;
	ma[1] = "abcd";
	ma[2] = "bcda";
	ma[3] = "cdab";
	ma[4] = "dabc";
	ma[5] = "1a";
	ma[6] = "2a";
	ma[7] = "3a";
	ma[8] = "4a";
	ma[9] = "5a";
	ma[10] = "6a";
	ma[-1] = "-!abcd";
	ma[-2] = "-!bcda";
	ma[-3] = "-!cdab";
	ma[-4] = "-!dabc";
	ma[-5] = "-!1a";
	ma[-6] = "-!2a";
	ma[-7] = "-!3a";
	ma[-8] = "-!4a";
	ma[-9] = "-!5a";
	ma[-10] = "-!6a";
	cout << ma << endl;
}