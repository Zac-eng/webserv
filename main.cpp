#include "location.hpp"

int main() {
	std::vector<LocationConfig> configList;
	
	parseConfFile("test.conf", configList);

	for (size_t i = 0; i < configList.size(); ++i) {
		const LocationConfig& cfg = configList[i];
		std::cout << "=== Location " << i + 1 << " ===" << std::endl;
		std::cout << "Path: " << cfg.path << std::endl;
		std::cout << "redirect_flag: " << (cfg.redirect_flag ? "true" : "false") << std::endl;

		std::map<int, std::string>::const_iterator it;
		for (it = cfg.redirect_map.begin(); it != cfg.redirect_map.end(); ++it) {
			std::cout << "Status code: " << it->first << ", Host: " << it->second << std::endl;
		}
		std::cout << std::endl;
	}

	return 0;
}
