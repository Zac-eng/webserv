#include "location.hpp"
#include "nginx.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <stdexcept>
#include <algorithm> // std::remove_if
#include <functional>

bool check_location(std::istream& config_file, LocationConfig& location_config)
{
	std::string line;
	bool has_closing = false;

	while (std::getline(config_file, line))
	{
		line = trim(line);

		if (line == "}")
		{
			has_closing = true;
			break;
		}

		if (line.empty())
			continue;

		std::stringstream stream(line);
		std::string keyword;
		stream >> keyword;

		if (keyword == "root")
		{
			std::string root_path;
			stream >> root_path;
			location_config.root = trim(root_path);
		}
		else if (keyword == "index")
		{
			std::string indexes;
			std::getline(stream, indexes);
			location_config.index = trim(indexes);
		}
		else if (keyword == "allow_methods")
		{
			std::string method;
			while (stream >> method)
			{
				location_config.addAllowMethod(method);
			}
		}
		else
		{
			std::cerr << "Error: Unknown directive in location block: " << line << std::endl;
			return false;
		}
	}

	if (!has_closing)
	{
		std::cerr << "Error: Missing closing '}' in location block." << std::endl;
		return false;
	}
	return true;
}
