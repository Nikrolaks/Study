#include <iostream>
#include <interalgo.h>

int main()
{
	map<int, char> m;

	m[3] = 'a';
	m[2] = 'b';
	m[7] = 'c';
	m[1] = 'd';
	m[12] = 'e';
	m[37] = 'f';

	std::cout << m << std::endl;
	system("pause");
}
