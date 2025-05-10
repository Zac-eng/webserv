#include "Server.hpp"
#include "ASocket.hpp"
#include "ListenSocket.hpp"
#include "CgiSocket.hpp"
#include "ClientSocket.hpp"
#include "Request.hpp"
#include "Response.hpp"

int Server::_epoll_fd;
std::map<int, ASocket*> Server::_socket;

int main(int argc, char* argv[]) {
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
                // std::cout << "      Allow Methods: ";
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
        return (0);
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
