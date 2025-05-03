#include <iostream>
#include <vector>
#include "nginx.hpp" // ServerConfig クラスと LocationConfig クラスを定義
#include "location.hpp"

// void remove_used_listen_ports(std::vector<int>& listen_ports, int count) {
// 	if (count > listen_ports.size())
// 		count = listen_ports.size();
// 	listen_ports.erase(listen_ports.begin(), listen_ports.begin() + count);
// }

// int main()
// {
// 	std::string config_filename = "nginx.conf"; // 設定ファイル名
// 	std::vector<ServerConfig> configs;
// 	ServerConfig config;
// 	int a;
// 	if (config.parse_config(config_filename, configs))
// 	{
// 		std::cout << "Config file parsed successfully!" << std::endl;
// 		size_t i = 0;
// 		while (i < configs.size())
// 		{
// 			const ServerConfig& server = configs[i];
// 			if (config.listen_counts[i] > 1)
// 			{
// 				for (a = 0; a < config.listen_counts[i]; a++)
// 				{
// 					std::cout << "Server " << a + i + 1 << ":" << std::endl;
// 					std::cout << "  Listen Port: " << config.listen_ports[a] << std::endl;
// 					std::cout << "  Server Name: " << server.server_name << std::endl;
// 					for (std::map<int, std::string>::const_iterator it = server.error_pages.begin(); it != server.error_pages.end(); ++it)
// 					{
// 						std::cout << "  Error Page: " << it->first << " -> " << it->second << std::endl;
// 					}
// 					std::cout << "	Root: " << config.root_server << std::endl;
// 					std::cout << "  Locations:" << std::endl;
// 					for (size_t j = 0; j < server.locations.size(); ++j)
// 					{
// 			   			const LocationConfig& loc = server.locations[j];
// 			   			std::cout << "		Location " << j + 1 << ":" << std::endl;
// 			   			std::cout << "		Path: " << loc.path << std::endl;
// 			   			std::cout << "		Root: " << loc.root << std::endl;
// 			   			std::cout << "		Index: ";
// 						if (!loc.index_push_count.empty())
// 						{
// 							size_t count = loc.index_push_count.back();
// 							size_t start = loc.index_files.size() >= count ? loc.index_files.size() - count : 0;
// 							for (size_t k = start; k < loc.index_files.size(); k++)
// 							{
// 								std::cout << loc.index_files[k];
// 								if (k < loc.index_files.size() - 1)
// 								std::cout << ", ";
// 							}
// 						}
// 						std::cout << std::endl;
// 						std::cout << "		Allow Methods: " << loc.method << std::endl;
// 						std::cout << "		fastcgi_index: " << loc.fastcgi_index << std::endl;
// 						std::cout << "		fastcgi_pass: " << loc.fastcgi_pass << std::endl;
// 						if (!loc.fastcgi_param.empty())
// 						{
// 							std::cout << "		fastcgi_params:" << std::endl;
// 							for (std::map<std::string, std::string>::const_iterator it = loc.fastcgi_param.begin();
// 								it != loc.fastcgi_param.end(); ++it)
// 							{
// 								std::cout << "			" << it->first << " : " << it->second << std::endl;
// 							}
// 						}
// 						std::cout << std::endl;
// 					}
// 				}
// 				i++;
// 				a -= 1;
// 			}
// 			else
// 			{
// 				std::cout << "Server " << a + i + 1 << ":" << std::endl;
// 				std::cout << "  Listen Port: " << server.listen_port << std::endl;
// 				std::cout << "  Server Name: " << server.server_name << std::endl;

// 				for (std::map<int, std::string>::const_iterator it = server.error_pages.begin(); it != server.error_pages.end(); ++it)
// 				{
// 					std::cout << "  Error Page: " << it->first << " -> " << it->second << std::endl;
// 				}
// 				std::cout << "	Root: " << server.root_server << std::endl;
// 				std::cout << "  Locations:" << std::endl;
// 				for (size_t j = 0; j < server.locations.size(); ++j)
// 				{
// 					const LocationConfig& loc = server.locations[j];
// 					std::cout << "		Location " << j + 1 << ":" << std::endl;
// 					std::cout << "		Path: " << loc.path << std::endl;
// 					std::cout << "		Root: " << loc.root << std::endl;
// 					std::cout << "		Index: ";
// 					if (!loc.index_push_count.empty())
// 					{
// 						size_t count = loc.index_push_count.back();
// 						size_t start = loc.index_files.size() >= count ? loc.index_files.size() - count : 0;
// 						for (size_t k = start; k < loc.index_files.size(); k++)
// 						{
// 							std::cout << loc.index_files[k];
// 							if (k < loc.index_files.size() - 1)
// 							std::cout << ", ";
// 						}
// 					}
// 					std::cout << std::endl;
// 					std::cout << "		Allow Methods: " << loc.method << std::endl;
// 					std::cout << "		fastcgi_index: " << loc.fastcgi_index << std::endl;
// 					std::cout << "		fastcgi_pass: " << loc.fastcgi_pass << std::endl;
// 					if (!loc.fastcgi_param.empty())
// 					{
// 						std::cout << "		fastcgi_params:" << std::endl;
// 						for (std::map<std::string, std::string>::const_iterator it = loc.fastcgi_param.begin();
// 							it != loc.fastcgi_param.end(); ++it)
// 						{
// 							std::cout << "			" << it->first << " : " << it->second << std::endl;
// 						}
// 					}
// 					// for (size_t k = 0; k < loc.allow_methods.size(); ++k)
// 					// {
// 					//	 std::cout << loc.allow_methods[k] << " ";
// 					// }
// 					std::cout << std::endl;
// 				}
// 				i++;
// 			}
// 		}
// 	}
// 	else
// 	{
// 		std::cerr << "Failed to parse config file." << std::endl;
// 	}

// 	return 0;
// }

void printServerConfig(const ServerConfig& server, int serverIndex, int listenPort) {
	std::cout << "Server " << serverIndex << ":" << std::endl;

	std::cout << "  Listen Port: " << listenPort << std::endl;
	std::cout << "  Server Name: " << server.getServerName() << std::endl;

	const std::map<int, std::string>& errorPages = server.getErrorPages();
	for (std::map<int, std::string>::const_iterator it = errorPages.begin(); it != errorPages.end(); ++it) {
		std::cout << "  Error Page: " << it->first << " -> " << it->second << std::endl;
	}

	std::cout << "  Root: " << server.getRootServer() << std::endl;

	std::cout << "  Locations:" << std::endl;
	for (size_t j = 0; j < server.getLocations().size(); ++j) {
		const LocationConfig& loc = server.getLocations()[j];
		std::cout << "    Location " << j + 1 << ":" << std::endl;
		std::cout << "      Path: " << loc.getPath() << std::endl;
		std::cout << "      Root: " << loc.getRoot() << std::endl;

		std::cout << "      Index: ";
		if (!loc.getIndexPushCount().empty()) {
			size_t count = loc.getIndexPushCount().back();
			const std::vector<std::string>& indexFiles = loc.getIndexFiles();
			size_t start = indexFiles.size() >= count ? indexFiles.size() - count : 0;
			for (size_t k = start; k < indexFiles.size(); k++) {
				std::cout << indexFiles[k];
				if (k < indexFiles.size() - 1)
					std::cout << ", ";
			}
		}
		std::cout << std::endl;

		std::cout << "      Allow Methods: " << loc.getMethod() << std::endl;
		std::cout << "      fastcgi_index: " << loc.getFastcgiIndex() << std::endl;
		std::cout << "      fastcgi_pass: " << loc.getFastcgiPass() << std::endl;

		if (!loc.getFastcgiParam().empty()) {
			std::cout << "      fastcgi_params:" << std::endl;
			for (std::map<std::string, std::string>::const_iterator it = loc.getFastcgiParam().begin();
				 it != loc.getFastcgiParam().end(); ++it) {
				std::cout << "        " << it->first << " : " << it->second << std::endl;
			}
		}

		std::cout << std::endl;
	}
}


int main() {
	std::string config_filename = "nginx.conf";
	std::vector<ServerConfig> configs;
	ServerConfig parser;

	if (parser.parse_config(config_filename, configs)) {
	std::cout << "Config file parsed successfully!" << std::endl;

	std::vector<int> listen_ports = parser.getListenPorts();
	std::vector<int> listen_counts = parser.getListenCounts();
	int portIndexOffset = 0;
	for (size_t i = 0; i < configs.size(); ++i) {
		int listenCount = listen_counts[i];
		for (int a = 0; a < listenCount; ++a) {
			int port = listen_ports[portIndexOffset + a];
			printServerConfig(configs[i], static_cast<int>(portIndexOffset + a + 1), port);
		}
		portIndexOffset += listenCount;
	}


	} else {
		std::cerr << "Failed to parse config file." << std::endl;
	}

	return 0;
}
