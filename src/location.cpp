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
	this->index_files = indexes;
}

void LocationConfig::setMethod(const std::vector<std::string>& methods)
{
	allow_method = methods;
}

void LocationConfig::reset() {
	root.clear();
	index_files.clear();
	allow_method.clear();
	fastcgi_index.clear();
	fastcgi_pass.clear();
	fastcgi_param.clear();
	redirect_map.clear();
	path_parser.clear();
}

bool LocationConfig::check_location(std::istream& config_file, LocationConfig& location_config)
{
	location_config.reset();
	std::string line;
	bool has_closing = false;

	while (std::getline(config_file, line))
	{
		line = trim(line);
		if (line.empty())
			continue;
		if (line == "}") {
			has_closing = true;
			break;
		}
		std::stringstream stream(line);
		std::string keyword;
		stream >> keyword;
		if (keyword == "root") {
			if (!handle_root(stream, location_config)) return false;
		} else if (keyword == "index") {
			if (!handle_index(stream, location_config)) return false;
		} else if (keyword == "autoindex") {
			if (!handle_autoindex(stream, location_config)) return false;
		} else if (keyword == "allow_methods") {
			if (!handle_allow_methods(stream, location_config)) return false;
		} else if (keyword == "fastcgi_index") {
			if (!handle_fastcgi_index(stream, location_config)) return false;
		} else if (keyword == "fastcgi_pass") {
			if (!handle_fastcgi_pass(stream, location_config)) return false;
		} else if (keyword == "fastcgi_param") {
			if (!handle_fastcgi_param(stream, location_config)) return false;
		} else if (keyword == "return") {
			if (!handle_return(stream, location_config)) return false;
		} else if (keyword == "path_parser") {
			if (!handle_path_parser(stream, location_config)) return false;
		} else {
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
