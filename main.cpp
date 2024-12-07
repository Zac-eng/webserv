#include "nginx.hpp"
#include <iostream>
#include <vector>

// int main()
// {
// 	std::vector<ServerConfig> configs;
// 	if (parse_config("nginx.conf", configs))
// 	{
// 		std::cout << "設定ファイルの解析に成功しました。" << std::endl;
// 		for (size_t i = 0; i < configs.size(); ++i)
// 		{
// 			std::cout << "server " << i + 1 << ": listen_port=" << configs[i].listen_port
// 					<< ", server_name=" << configs[i].server_name << std::endl;
// 		}
// 	}
// 	return 0;
// }

int main() {
    std::vector<ServerConfig> configs;
    std::string filename = "nginx.conf";

    if (parse_config(filename, configs)) {
        std::cout << "Configuration parsed successfully!" << std::endl;
        for (size_t i = 0; i < configs.size(); ++i) {
            std::cout << "Server " << i + 1 << ":" << std::endl;
            std::cout << "  Listen Port: " << configs[i].listen_port << std::endl;
            std::cout << "  Server Name: " << configs[i].server_name << std::endl;
        }
    } else {
        std::cerr << "Failed to parse configuration." << std::endl;
    }

    return 0;
}