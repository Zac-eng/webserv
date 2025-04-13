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

// bool check_location(std::istream& config_file, LocationConfig& location_config)
// {
// 	std::string line;
// 	bool has_closing = false;

// 	while (std::getline(config_file, line))
// 	{
// 		line = trim(line);

// 		if (line == "}")
// 		{
// 			has_closing = true;
// 			break;
// 		}

// 		if (line.empty())
// 			continue;

// 		std::stringstream stream(line);
// 		std::string keyword;
// 		stream >> keyword;

// 		if (keyword == "root")
// 		{
// 			std::string root_path;
// 			stream >> root_path;
// 			location_config.root = trim(root_path);
// 		}
// 		else if (keyword == "index")
// 		{
// 			std::string indexes;
// 			std::getline(stream, indexes);
// 			std::cout << "indexes: " << indexes << std::endl;
// 			location_config.index = trim(indexes);
// 			std::cout << "index: " << location_config.index << std::endl;
// 		}
// 		else if (keyword == "allow_methods")
// 		{
// 			std::string method;
// 			std::getline(stream, method);
// 			location_config.method = trim(method);
// 		}
// 		else
// 		{
// 			std::cerr << "Error: Unknown directive in location block: " << line << std::endl;
// 			return false;
// 		}
// 	}

// 	if (!has_closing)
// 	{
// 		std::cerr << "Error: Missing closing '}' in location block." << std::endl;
// 		return false;
// 	}
// 	return true;
// }

void LocationConfig::setIndex(const std::vector<std::string>& indexes)
{
	index_files = indexes;
}

bool LocationConfig::check_location(std::istream& config_file, LocationConfig& location_config)
{
	std::string line;
	bool has_closing = false;
	size_t prev_size = index_files.size();
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
			if (!root_path.empty() && root_path.back() == ';')
				root_path.pop_back();
			else
			{
				std::cerr << "Error: Missing semicolon after 'root' directive." << std::endl;
				return false;
			}
			if (root_path.back() == '/')
			 root_path.pop_back();
			location_config.root = trim(root_path);
		}
		else if (keyword == "index")
		{
			std::string file;
			while (stream >> file)
			{
				if (file.back() == ';')
				{
					file.pop_back();
					index_files.push_back(file);
					break;
				}
				index_files.push_back(file);
			}
			if (index_files.size() >= prev_size)
			{
				index_push_count.push_back(index_files.size() - prev_size);
			}
			location_config.setIndex(index_files);
		}
		else if (keyword == "allow_methods")
		{
			std::string method;
			std::getline(stream, method);
			location_config.method = trim(method);
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
