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
		ServerConfig conf("8080", "localhost");
		Server server(nginx);
		if (server.ServerCreate() == false);
			return (1);
		if (server.ExecuteLoop() == false)
		return (false);
	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() << std::endl;
		return 1;
	}
}
