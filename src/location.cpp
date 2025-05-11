#include "location.hpp"
#include "nginx.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <stdexcept>
#include <algorithm>
#include <functional>

void LocationConfig::setIndex(const std::vector<std::string>& indexes)
{
	index_files = indexes;
}

void LocationConfig::setMethod(const std::vector<std::string>& methods)
{
	allow_method = methods;
}

bool LocationConfig::check_location(std::istream& config_file, LocationConfig& location_config)
{
	std::string line;
	bool has_closing = false;
	size_t prev_size = index_files.size();
	size_t method_prev = allow_method.size();
	while (std::getline(config_file, line))
	{
		line = trim(line);

		if (line == "}") {
			has_closing = true;
			break;
		}
		if (line.empty())
			continue;
		std::stringstream stream(line);
		std::string keyword;
		stream >> keyword;
		if (keyword == "root") {
			std::string root_path;
			stream >> root_path;
			if (!root_path.empty() && root_path[root_path.size() - 1] == ';')
				root_path.erase(root_path.size() - 1);
			else {
				std::cerr << "Error: Missing semicolon after 'root' directive." << std::endl;
				return false;
			}
			if (!root_path.empty() && root_path[root_path.size() - 1] == '/')
				root_path.erase(root_path.size() - 1);
			location_config.root = trim(root_path);
		}
		else if (keyword == "index") {
			std::string file;
			while (stream >> file)
			{
				if (!file.empty() && file[file.size() - 1] == ';') {
					file.erase(file.size() - 1);
					index_files.push_back(file);
					break;
				}
				index_files.push_back(file);
			}
			if (index_files.size() >= prev_size)
				index_push_count.push_back(index_files.size() - prev_size);
			location_config.setIndex(index_files);
		}
		else if (keyword == "allow_methods") {
			std::string method;
			while (stream >> method)
			{
				if (!method.empty() && method[method.size() - 1] == ';') {
					method.erase(method.size() - 1);
					allow_method.push_back(method);
					break;
				}
				allow_method.push_back(method);
			}
			if (allow_method.size() >= method_prev)
				allow_method_count.push_back(allow_method.size() - method_prev);
			location_config.setMethod(allow_method);
		}
		else if (keyword == "fastcgi_index") {
			std::string cgi_index;
			stream >> cgi_index;
			if (!cgi_index.empty() && cgi_index[cgi_index.size() - 1] == ';')
				cgi_index.erase(cgi_index.size() - 1);
			else {
				std::cerr << "Error: Missing semicolon after 'fastcgi_index' directive." << std::endl;
				return false;
			}
			location_config.fastcgi_index = trim(cgi_index);
		}
		else if (keyword == "fastcgi_pass") {
			std::string cgi_pass;
			stream >> cgi_pass;
			if (!cgi_pass.empty() && cgi_pass[cgi_pass.size() - 1] == ';')
				cgi_pass.erase(cgi_pass.size() - 1);
			else {
				std::cerr << "Error: Missing semicolon after 'fastcgi_pass' directive." << std::endl;
				return false;
			}
			location_config.fastcgi_pass = trim(cgi_pass);
		}
		else if (keyword == "fastcgi_param") {
			std::string param_first;
			std::string param_second;
			stream >> param_first;
			if (param_first.empty()) {
				std::cerr << "Error: Missing param in 'fastcgi_param' directive." << std::endl;
				return false;
			}
			std::getline(stream, param_second);
			param_second = trim(param_second);
			if (!param_second.empty() && param_second[param_second.size() - 1] == ';')
				param_second.erase(param_second.size() - 1);
			else {
				std::cerr << "Error: Missing semicolon after 'fastcgi_param' directive." << std::endl;
				return false; 
			}
			location_config.fastcgi_param[param_first] = param_second;
		}
		else {
			std::cerr << "Error: Unknown directive in location block: " << line << std::endl;
			return false;
		}
	}
	if (!has_closing) {
		std::cerr << "Error: Missing closing '}' in location block." << std::endl;
		return false;
	}
	return true;
}
