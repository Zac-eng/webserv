#include <iostream>
#include <string>
#include <vector>
#include "Server.hpp"
#include "Request.hpp"
#include "Socket.hpp"
#include "Client.hpp"
#include "nginx.hpp"

int main()
{
	// if (ac != 2) {
	// 	std::cerr << "Invalid arguments" << std::endl;
	// 	return 1;
	// }
	try
	{
		// std::string message = av[1];
		// Class nginx(message);
		// nginx_parse(nginx);
		ServerConfig conf(8080, "127.0.0.1");
		std::vector<ServerConfig> confs;
		confs.push_back(conf);
		Server server(confs);
		if (server.ServerCreate() == false)
			return (1);
	// debug();

		if (server.ExecuteLoop() == false)
		return (1);

	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() << std::endl;
		return 1;
	}
}
