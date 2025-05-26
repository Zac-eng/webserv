#include "location.hpp"

void LocationConfig::setPathParser(const std::string& path) {
	path_parser = path;
}

void LocationConfig::setRedirectFlag(bool flag) {
	redirect_flag = flag;
}

void LocationConfig::setRedirectMap(int status_code, const std::string& url) {
	redirect_map[status_code] = url;
}

const std::string& LocationConfig::getPathParser() const {
	return path_parser;
}

const std::map<int, std::string>& LocationConfig::getRedirectMap() const {
	return redirect_map;
}

bool LocationConfig::getRedirectFlag() const {
	return redirect_flag;
}