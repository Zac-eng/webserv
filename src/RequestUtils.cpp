#include "Request.hpp"
#include "ClientSocket.hpp"

namespace Error
{
	void MissingRequestLineAndHost(void)
	{
		std::cout << "Error: Missing RequestLine or Host" << std::endl;
		return ;
	}
	void InvalidMethod(void)
	{
		std::cout << "Error: Invalid method" << std::endl;
		return ;
	}
	void InvalidUri(void)
	{
		std::cout << "Error: Invalid uri" << std::endl;
		return ;
	}
	void InvalidVersion(void)
	{
		std::cout << "Error: Invalid veersion" << std::endl;
		return ;
	}
	void InvalidRequestLine(void)
	{
		std::cout << "Error: Invalid request line" << std::endl;
		return ;
	}
	void ReadingBuffer(void)
	{
		std::cout << "Error: Invalid request line" << std::endl;
		return ;
	}
}

bool SkipSpaceAndCheckEnd(const std::string& request, std::string::const_iterator& it)
{
	while (it != request.end() && std::isspace(*it))
		it++;
	if (it == request.end())
		return (false);
	return (true);
}

bool GetSubstringUntilSpace(const std::string& request, std::string::const_iterator& it, std::string& object)
{
	size_t pos = 0;

	pos = request.find(' ', it - request.begin());
	if (pos == std::string::npos)
		return (false);
	object = std::string(it, request.begin() + pos);
	it += object.length() + 1;
	return (true);
} 

bool GetSubstringUntilCarriageReturn(const std::string& request, std::string::const_iterator& it, std::string& object)
{
	size_t pos = 0;

	pos = request.find("\r\n", it - request.begin());
	if (pos == std::string::npos)
		return (false);
	object = request.substr(it - request.begin(), pos - (it - request.begin()));
	//現在地から、\r\nが見つかった場所-現在地の要素数分切り取る。
	it += object.length() + 2;
	return (true);
}

bool substring_object_until_carrige_return(std::string& buffer, std::string::iterator& it, std::string& object)
{
	size_t pos = 0;

	//posはbuuferのindexを表す。
	pos = buffer.find("\r\n", it - buffer.begin());
	if (pos == std::string::npos)
		return (false);
	object = buffer.substr(it - buffer.begin(), pos - (it - buffer.begin()) + 2);
	it = buffer.begin();
	it += pos + 2;
	return (true);
}

size_t string_to_int(const std::string &str)
{
	std::stringstream convert_int(str);
	int num;
	convert_int >> num; 
	if (convert_int.fail())
	{
		return (-1);
	}
	return (num);
}

void debug(const std::string& str)
{
	std::cout << str <<std::endl;
}