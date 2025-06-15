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

bool LocationConfig::handle_root(std::stringstream& stream, LocationConfig& location_config) {
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
	return true;
}

bool LocationConfig::handle_index(std::stringstream& stream, LocationConfig& location_config) {
	std::string file;
	size_t prev_size = index_files.size();
	while (stream >> file) {
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
	return true;
}

bool LocationConfig::handle_autoindex(std::stringstream& stream, LocationConfig& location_config) {
	std::string autoindex_path;
	stream >> autoindex_path;
	if (!autoindex_path.empty() && autoindex_path[autoindex_path.size() - 1] == ';')
		autoindex_path.erase(autoindex_path.size() - 1);
	else {
		std::cerr << "Error: Missing semicolon after 'autoindex' directive." << std::endl;
		return false;
	}
	if (!autoindex_path.empty() && autoindex_path[autoindex_path.size() - 1] == '/')
		autoindex_path.erase(autoindex_path.size() - 1);
	location_config.autoindex = trim(autoindex_path);
	if (location_config.autoindex == "on") {
		on_off = true;
	} else if (location_config.autoindex == "off") {
		on_off = false;
	} else {
		std::cerr << "Error: Invalid value for 'autoindex'. Expected 'on' or 'off', but got '" 
			<< location_config.autoindex << "'." << std::endl;
		return false;
	}
	return true;
}

bool LocationConfig::handle_allow_methods(std::stringstream& stream, LocationConfig& location_config) {
	std::string method;
	size_t method_prev = allow_method.size();
	while (stream >> method) {
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
	return true;
}

bool LocationConfig::handle_fastcgi_index(std::stringstream& stream, LocationConfig& location_config) {
	std::string cgi_index;
	stream >> cgi_index;
	if (!cgi_index.empty() && cgi_index[cgi_index.size() - 1] == ';')
		cgi_index.erase(cgi_index.size() - 1);
	else {
		std::cerr << "Error: Missing semicolon after 'fastcgi_index' directive." << std::endl;
		return false;
	}
	location_config.fastcgi_index = trim(cgi_index);
	return true;
}

bool LocationConfig::handle_fastcgi_pass(std::stringstream& stream, LocationConfig& location_config) {
	std::string cgi_pass;
	stream >> cgi_pass;
	if (!cgi_pass.empty() && cgi_pass[cgi_pass.size() - 1] == ';')
		cgi_pass.erase(cgi_pass.size() - 1);
	else {
		std::cerr << "Error: Missing semicolon after 'fastcgi_pass' directive." << std::endl;
		return false;
	}
	location_config.fastcgi_pass = trim(cgi_pass);
	return true;
}

bool LocationConfig::handle_fastcgi_param(std::stringstream& stream, LocationConfig& location_config) {
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
	return true;
}

bool LocationConfig::handle_return(std::stringstream& stream, LocationConfig& location_config) {
	std::string code_str, url;
	stream >> code_str >> url;

	if (url.empty()) {
		std::cerr << "Error: Missing URL in 'return' directive." << std::endl;
		return false;
	}

	if (!url.empty() && url[url.length() - 1] == ';') {
		url.erase(url.length() - 1);
	} else {
		std::cerr << "Error: Missing semicolon after 'return' directive." << std::endl;
		return false;
	}

	int code;
	try {
		code = my_stoi(code_str);
		if (code < 100 || code > 599) {
			std::cerr << "Error: Invalid HTTP status code in 'return': " << code_str << std::endl;
			return false;
		}
	} catch (...) {
		std::cerr << "Error: Non-numeric status code in 'return': " << code_str << std::endl;
		return false;
	}

	location_config.redirect_flag = true;
	location_config.redirect_map[code] = url;
	return true;
}

bool LocationConfig::handle_path_parser(std::stringstream& stream, LocationConfig& location_config) {
	std::string path_parser;
	stream >> path_parser;
	if (!path_parser.empty() && path_parser[path_parser.size() - 1] == ';')
		path_parser.erase(path_parser.size() - 1);
	else {
		std::cerr << "Error: Missing semicolon after 'path_parser' directive." << std::endl;
		return false;
	}
	location_config.path_parser = trim(path_parser);
	return true;
}
