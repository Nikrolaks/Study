#include <iostream>
#include <interalgo.h>

using namespace std;

int main()
{
	simple_treap<int> tr({ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 });

	cout << tr << endl;
}
