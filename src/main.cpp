#include <iostream>
#include <string>
#include <vector>
#include "Server.hpp"
#include "Request.hpp"

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
		Server server;
		server.ServerRequest();
	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() << std::endl;
		return 1;
	}
}

