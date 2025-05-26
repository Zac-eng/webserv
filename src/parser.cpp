#include "location.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>

std::string extractHost(const std::string& url) {
	size_t protocol_pos = url.find("://");
	if (protocol_pos == std::string::npos) {
		std::cerr << "Invalid URL (missing protocol): " << url << std::endl;
		return "";
	}

	size_t start = protocol_pos + 3;
	size_t first_slash = url.find('/', start);
	if (first_slash == std::string::npos) {
		std::cerr << "Invalid URL (missing slash after host): " << url << std::endl;
		return "";
	}

	std::string path = url.substr(first_slash + 1);
	if (path.empty()) {
		std::cerr << "Invalid URL (no path after slash): " << url << std::endl;
		return "";
	}

	return path;
}

void LocationConfig::parseReturnDirective(const std::string& line, LocationConfig& config) {
	std::istringstream iss(line);
	std::string keyword;
	int status_code;
	std::string url;

	iss >> keyword >> status_code >> url;

	if (keyword == "return" && !url.empty()) {
		if (url[url.length() - 1] == ';')
			url.erase(url.length() - 1);

		config.redirect_flag = true;
		config.redirect_map[status_code] = url;  // URL全体を保存
	}
}


std::string extractLocationPath(const std::string& line) {
	std::istringstream iss(line);
	std::string keyword, path;
	iss >> keyword >> path;
	return path;
}

void LocationConfig::parseConfFile(const std::string& filename, std::vector<LocationConfig>& configs) {
	std::ifstream infile(filename.c_str());
	std::string line;
	LocationConfig current;
	bool in_location = false;

	while (std::getline(infile, line)) {
		if (line.find_first_not_of(" \t\n\r") == std::string::npos)
			continue;
		if (line.find("location") != std::string::npos) {
			current = LocationConfig();
			current.path = extractLocationPath(line);
			in_location = true;
		} else if (in_location && line.find("return") != std::string::npos) {
			parseReturnDirective(line, current);
		} else if (in_location && line.find("}") != std::string::npos) {
			configs.push_back(current);
			in_location = false;
		}
	}
}


// int main() {
// 	std::vector<LocationConfig> configList;
	
// 	parseConfFile("test.conf", configList);

// 	for (size_t i = 0; i < configList.size(); ++i) {
// 		const LocationConfig& cfg = configList[i];
// 		std::cout << "=== Location " << i + 1 << " ===" << std::endl;
// 		std::cout << "Path: " << cfg.path_parser << std::endl;
// 		std::cout << "redirect_flag: " << (cfg.redirect_flag ? "true" : "false") << std::endl;

// 		std::map<int, std::string>::const_iterator it;
// 		for (it = cfg.redirect_map.begin(); it != cfg.redirect_map.end(); ++it) {
// 			std::cout << "Status code: " << it->first << ", Host: " << it->second << std::endl;
// 		}
// 		std::cout << std::endl;
// 	}

// 	return 0;
// }
