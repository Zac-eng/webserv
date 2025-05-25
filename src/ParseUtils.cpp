#include "ParseUtils.hpp"

bool ParseUtils::parse_object(std::string& input, const std::string& object)
{
	if (input.substr(0, object.length()) == object)
	{
		input = input.substr(object.length());
		return (true);
	}
	return (false);
}

bool ParseUtils::check_valid_version(std::string& input, const std::string& object)
{
	std::string::iterator it;
	bool period_flag;

	period_flag = false;
	it = input.begin();
	if (input.substr(0, object.length()) == object)
	{
		input = input.substr(object.length());
		return (true);
	}
	for (; it != input.end() || *it != ' '; it++)
	{
		if (std::isdigit(*it) == false)
		{
			if (*it == '.')
			{
				if (period_flag == true)
					return (false);
				else
				{
					period_flag = true;
					continue ;
				}
			}
			return (false);
		}
	}
	for (; it != input.end(); it++)
	{
		if (*it != ' ')
			return (false);
	}
	if (it == input.end())
		throw RequestException(505, "version");
	return (false);
}
