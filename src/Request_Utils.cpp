#include "Request.hpp"

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
	object = request.substr(it - request.begin(), pos - (it - request.begin()));
	it += pos + 1;
	return (true);
}

bool GetSubstringUntilCarriageReturn(const std::string& request, std::string::const_iterator& it, std::string& object)
{
	size_t pos = 0;

	pos = request.find('\r\n', it - request.begin());
	if (pos == std::string::npos)
		return (false);
	object = request.substr(it - request.begin(), pos - (it - request.begin()));
	it += pos + 2;
	return (true);
}