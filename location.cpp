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
		else if (keyword == "fastcgi_index")
		{
			std::cout << "----------------\n";
			std::string cgi_index;
			stream >> cgi_index;
			if (!cgi_index.empty() && cgi_index.back() == ';')
				cgi_index.pop_back();
			else
			{
				std::cerr << "Error: Missing semicolon after 'fastcgi_index' directive." << std::endl;
				return false;
			}
			location_config.fastcgi_index = trim(cgi_index);
		
		}
		else if (keyword == "fastcgi_pass")
		{
			std::string cgi_pass;
			stream >> cgi_pass;
			if (!cgi_pass.empty() && cgi_pass.back() == ';')
				cgi_pass.pop_back();
			else
			{
				std::cerr << "Error: Missing semicolon after 'fastcgi_pass' directive." << std::endl;
				return false;
			}
			location_config.fastcgi_pass = trim(cgi_pass);
		}
		else if (keyword == "fastcgi_param")
		{
			std::string param_first;
			std::string param_second;
			stream >> param_first;
			if (param_first.empty())
			{
				std::cerr << "Error: Missing param in 'fastcgi_param' directive." << std::endl;
				return false;
			}
			std::getline(stream, param_second);
			param_second = trim(param_second);
			if (!param_second.empty() && param_second.back() == ';')
				param_second.pop_back();
			else
			{
				std::cerr << "Error: Missing semicolon after 'fastcgi_param' directive." << std::endl;
				return false; 
			}
			location_config.fastcgi_param[param_first] = param_second;
			std::cout << "[Debug] fastcgi_params contents:" << std::endl;
	for (std::map<std::string, std::string>::iterator it = location_config.fastcgi_param.begin();
		 it != location_config.fastcgi_param.end(); ++it)
	{
		std::cout << "  key: [" << it->first << "] value: [" << it->second << "]" << std::endl;
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
