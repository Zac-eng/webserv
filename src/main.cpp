#include <iostream>
#include <string>
#include <vector>
#include "Server.hpp"
#include "Request.hpp"
#include "Socket.hpp"
#include "Client.hpp"
#include "nginx.hpp"
#include "location.hpp"

int main()
{
    std::string config_filename = "nginx.conf"; // 設定ファイル名
    std::vector<ServerConfig> configs;

    if (parse_config(config_filename, configs))
    {
        std::cout << "Config file parsed successfully!" << std::endl;

        for (size_t i = 0; i < configs.size(); ++i)
        {
            const ServerConfig& server = configs[i];
            std::cout << "Server " << i + 1 << ":" << std::endl;
            std::cout << "  Listen Port: " << server.listen_port << std::endl;
            std::cout << "  Server Name: " << server.server_name << std::endl;

            for (std::map<int, std::string>::const_iterator it = server.error_pages.begin(); it != server.error_pages.end(); ++it)
            {
                std::cout << "  Error Page: " << it->first << " -> " << it->second << std::endl;
            }

            std::cout << "  Locations:" << std::endl;
            for (size_t j = 0; j < server.locations.size(); ++j)
            {
                const LocationConfig& loc = server.locations[j];
                std::cout << "    Location " << j + 1 << ":" << std::endl;
                std::cout << "      Path: " << loc.path << std::endl;
                std::cout << "      Root: " << loc.root << std::endl;
                std::cout << "      Index: " << loc.index << std::endl;
                std::cout << "      Allow Methods: ";
                for (size_t k = 0; k < loc.allow_methods.size(); ++k)
                {
                    std::cout << loc.allow_methods[k] << " ";
                }
                std::cout << std::endl;
            }
        }
    }
    else
    {
        std::cerr << "Failed to parse config file." << std::endl;
    }
	// if (ac != 2) {
	// 	std::cerr << "Invalid arguments" << std::endl;
	// 	return 1;
	// }
	try
	{
		// std::string message = av[1];
		// Class nginx(message);
		// nginx_parse(nginx);
		Server server(configs);
		// for (int i = 0; i < confs.size(); i++)
		// {
		// 	if (server.ServerCreate(confs[i]) == false)
		// 		return (1);
		// }
		if (server.ServerCreate() == false)
			return (1);
	// debug();

		if (server.ExecuteLoop() == false)
		return (1);

	}
	catch (const std::exception &e)
	{
		// server.CloseEpollFd();
		std::cerr << "Error: " << e.what() << std::endl;
		return 1;
	}
}



// #include <iostream>
// #include <string>
// #include <vector>
// #include "Server.hpp"
// #include "Request.hpp"
// #include "Socket.hpp"
// #include "Client.hpp"
// #include "nginx.hpp"

// int main()
// {
// 	// if (ac != 2) {
// 	// 	std::cerr << "Invalid arguments" << std::endl;
// 	// 	return 1;
// 	// }
// 	try
// 	{
// 		// std::string message = av[1];
// 		// Class nginx(message);
// 		// nginx_parse(nginx);
// 		ServerConfig conf(8080, "127.0.0.1");
// 		std::vector<ServerConfig> confs;
// 		confs.push_back(conf);
// 		Server server(confs);
// 		// for (int i = 0; i < confs.size(); i++)
// 		// {
// 		// 	if (server.ServerCreate(confs[i]) == false)
// 		// 		return (1);
// 		// }
// 		if (server.ServerCreate() == false)
// 			return (1);
// 	// debug();

// 		if (server.ExecuteLoop() == false)
// 		return (1);

// 	}
// 	catch (const std::exception &e)
// 	{
// 		// server.CloseEpollFd();
// 		std::cerr << "Error: " << e.what() << std::endl;
// 		return 1;
// 	}
// }
