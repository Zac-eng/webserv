#include <iostream>
#include <vector>
#include "nginx.hpp" // ServerConfig クラスと LocationConfig クラスを定義
#include "location.hpp"

int main()
{
    std::string config_filename = "nginx.conf"; // 設定ファイル名
    std::vector<ServerConfig> configs;
    ServerConfig config;
	int a;
    if (config.parse_config(config_filename, configs))
    {
        std::cout << "Config file parsed successfully!" << std::endl;
		size_t i = 0;
        while (i < configs.size())
        {
            const ServerConfig& server = configs[i];
			if (config.listen_counts[i] > 1)
			{
				for (a = 0; a < config.listen_counts[i]; a++)
				{
					std::cout << "Server " << a + i + 1 << ":" << std::endl;
					std::cout << "  Listen Port: " << config.listen_ports[a] << std::endl;
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
               			std::cout << "      Index: ";
						if (!loc.index_push_count.empty())
						{
							size_t count = loc.index_push_count.back();
							size_t start = loc.index_files.size() >= count ? loc.index_files.size() - count : 0;
							for (size_t k = start; k < loc.index_files.size(); k++)
							{
								std::cout << loc.index_files[k];
								if (k < loc.index_files.size() - 1)
								std::cout << ", ";
							}
						}
						std::cout << std::endl;
                		std::cout << "      Allow Methods: " << loc.method << std::endl;
                		// for (size_t k = 0; k < loc.allow_methods.size(); ++k)
                		// {
                		//     std::cout << loc.allow_methods[k] << " ";
            			// }
                		std::cout << std::endl;
            		}
				}
				i++;
				a -= 1;
			}
            else
			{
				std::cout << "Server " << a + i + 1 << ":" << std::endl;
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
            	    std::cout << "      Index: ";
					if (!loc.index_push_count.empty())
					{
						size_t count = loc.index_push_count.back();
						size_t start = loc.index_files.size() >= count ? loc.index_files.size() - count : 0;
						for (size_t k = start; k < loc.index_files.size(); k++)
						{
							std::cout << loc.index_files[k];
							if (k < loc.index_files.size() - 1)
							std::cout << ", ";
						}
					}
					std::cout << std::endl;
            	    std::cout << "      Allow Methods: " << loc.method << std::endl;
                	// for (size_t k = 0; k < loc.allow_methods.size(); ++k)
                	// {
                	//     std::cout << loc.allow_methods[k] << " ";
                	// }
                	std::cout << std::endl;
            	}
				i++;
			}
        }
    }
    else
    {
        std::cerr << "Failed to parse config file." << std::endl;
    }

    return 0;
}
