#include <iostream>
#include <interalgo.h>

using namespace std;

int main()
{
	dictionary<int, string> dict;

	dict[1] = "hello";
	dict[4] = "goodbye";
	dict[3] = "mr";
	dict[2] = "Robot";

	cout << dict << endl;
}
