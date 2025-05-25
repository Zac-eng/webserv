#ifndef LOCATION_HPP
#define LOCATION_HPP

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>

class LocationConfig {
	private:
	
	public:
		std::string path;
		bool redirect_flag;
		std::map<int, std::string> redirect_map;
		LocationConfig() : path(""), redirect_flag(false) {}
};
void parseReturnDirective(const std::string& line, LocationConfig& config);
void parseConfFile(const std::string& filename, std::vector<LocationConfig>& configs);

#endif