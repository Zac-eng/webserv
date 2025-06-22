#include "Server.hpp"
#include "ASocket.hpp"
#include "ListenSocket.hpp"
#include "CgiSocket.hpp"
#include "ClientSocket.hpp"
#include "Request.hpp"
#include "Response.hpp"
#include "nginx.hpp"
#include "location.hpp"
#include "Signal.hpp"


int main(int argc, char **argv) {
	if (argc != 2) {
		std::cerr << "Usage: " << argv[0] << " <config_file>" << std::endl;
		return 1;
	}
	signal(SIGCHLD, sigchld_handler);
	std::string config_filename = argv[1];
	std::vector<ServerConfig> configs;
	ServerConfig parser;

	if (!parser.parse_config(config_filename, configs)) {
		std::cerr << "Failed to parse config file." << std::endl;
		return 1;
	}
    try
    {
		Server server(configs);
        server.createListenServer();
		std::signal(SIGINT, Signal::signal_handler);
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
