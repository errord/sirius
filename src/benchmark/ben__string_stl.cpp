#include <string>
#include <iostream>


int main()
{
	std::string s1;
	std::string s2("我是中国人!!!");
	size_t i, n;
	n = 100000000;
	for (i = 0; i < n; ++i) {
		s1.append(s2);
	}
	std::cout << "length:" << s1.length() << std::endl;
	return 0;
}

