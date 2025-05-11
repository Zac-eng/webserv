#include "Server.hpp"
#include "ASocket.hpp"
#include "ListenSocket.hpp"
#include "CgiSocket.hpp"
#include "ClientSocket.hpp"
#include "Request.hpp"
#include "Response.hpp"
#include "nginx.hpp" // ServerConfig クラスと LocationConfig クラスを定義
#include "location.hpp"

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
		std::cout << "      Allow Methods: ";
		if (!loc.getMethodCount().empty()) {
			size_t count = loc.getMethodCount().back();
			const std::vector<std::string>& methods = loc.getMethod();
			size_t start = methods.size() >= count ? methods.size() - count : 0;
			for (size_t k = start; k < methods.size(); k++) {
				std::cout << methods[k];
				if (k < methods.size() - 1)
					std::cout << ", ";
			}
		}
		std::cout << std::endl;
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
    try
    {
        Server server(configs);
        server.createListenServer();
        server.executeServer();
    }
    catch (const ServerException& e)
    {
        std::cout << e.what() << std::endl;
    }
    catch (std::exception& e)
    {
        std::cout << e.what() << std::endl;
    }
}
