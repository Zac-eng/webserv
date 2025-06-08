#include <iostream>
#include <cstring>

int main(void)
{
	std::string str;
	std::string::iterator it;


	str = "iteratir\r\n";
	it = str.begin();

	for (; it != str.end() && *it != '\r';it++)
	{
		std::cout <<*it<<std::endl;
	}
	if (*it != '\r')
	{
		std::cout << *it << ":owari"<<std::endl;
	}
	std::cout << *it<<std::endl;
}