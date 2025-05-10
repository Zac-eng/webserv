#include "location.hpp"
#include <iostream>

// Setters
void LocationConfig::setPath(const std::string& p) {
	path = p;
}

void LocationConfig::setRoot(const std::string& r) {
	root = r;
}

void LocationConfig::setMethod(const std::string& m) {
	method = m;
}

void LocationConfig::setFastcgiIndex(const std::string& idx) {
	fastcgi_index = idx;
}

void LocationConfig::setFastcgiPass(const std::string& pass) {
	fastcgi_pass = pass;
}


void LocationConfig::setIndexCount(const std::vector<int>& counts) {
	index_count = counts;
}

void LocationConfig::setIndexPushCount(const std::vector<int>& counts) {
	index_push_count = counts;
}

void LocationConfig::setFastcgiParam(const std::map<std::string, std::string>& params) {
	fastcgi_param = params;
}

// Getters
const std::string& LocationConfig::getPath() const {
	return path;
}

const std::string& LocationConfig::getRoot() const {
	return root;
}

const std::string& LocationConfig::getMethod() const {
	return method;
}

const std::string& LocationConfig::getFastcgiIndex() const {
	return fastcgi_index;
}

const std::string& LocationConfig::getFastcgiPass() const {
	return fastcgi_pass;
}

const std::vector<std::string>& LocationConfig::getIndexFiles() const {
	return index_files;
}

const std::vector<int>& LocationConfig::getIndexCount() const {
	return index_count;
}

const std::vector<int>& LocationConfig::getIndexPushCount() const {
	return index_push_count;
}

const std::map<std::string, std::string>& LocationConfig::getFastcgiParam() const {
	return fastcgi_param;
}

