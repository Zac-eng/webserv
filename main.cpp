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

// int main() {
//     std::vector<ServerConfig> configs;
//     std::string filename = "nginx.conf";

//     if (parse_config(filename, configs)) {
//         std::cout << "Configuration parsed successfully!" << std::endl;
//         for (size_t i = 0; i < configs.size(); ++i) {
//             std::cout << "Server " << i + 1 << ":" << std::endl;
//             std::cout << "  Listen Port: " << configs[i].listen_port << std::endl;
//             std::cout << "  Server Name: " << configs[i].server_name << std::endl;
//         }
//     } else {
//         std::cerr << "Failed to parse configuration." << std::endl;
//     }

//     return 0;
// }
// int main() {
//     std::vector<ServerConfig> configs; // サーバー設定情報を格納するベクター
//     std::string filename = "nginx.conf"; // 設定ファイルの名前

//     // 設定ファイルの解析を試みる
//     if (parse_config(filename, configs)) {
//         std::cout << "Configuration parsed successfully!" << std::endl;

//         // 各サーバー設定情報を出力する
//         for (size_t i = 0; i < configs.size(); ++i) {
//             std::cout << "Server " << i + 1 << ":" << std::endl;
//             std::cout << "  Listen Port: " << configs[i].listen_port << std::endl;
//             std::cout << "  Server Name: " << configs[i].server_name << std::endl;

//             // error_page の情報を表示する
//             if (!configs[i].error_pages.empty()) {
//                 std::cout << "  Error Pages:" << std::endl;
//                 for (std::map<int, std::string>::const_iterator it = configs[i].error_pages.begin();
//                      it != configs[i].error_pages.end(); ++it) {
//                     std::cout << "    Error Code " << it->first << " -> " << it->second << std::endl;
//                 }
//             } else {
//                 std::cout << "  Error Pages: None" << std::endl;
//             }
//         }
//     } else {
//         // 解析に失敗した場合のエラーメッセージ
//         std::cerr << "Failed to parse configuration." << std::endl;
//     }

//     return 0;
// }

#include <iostream>
#include <vector>
#include "nginx.hpp" // ServerConfig クラスと LocationConfig クラスを定義
#include "location.hpp"

// int main() {
//     std::vector<ServerConfig> configs; // サーバー設定情報を格納するベクター
//     std::string filename = "nginx.conf"; // 設定ファイルの名前

//     // 設定ファイルの解析を試みる
//     if (parse_config(filename, configs)) {
//         std::cout << "Configuration parsed successfully!" << std::endl;

//         // 各サーバー設定情報を出力する
//         for (size_t i = 0; i < configs.size(); ++i) {
//             std::cout << "\nServer " << i + 1 << ":" << std::endl;
//             std::cout << "  Listen Port: " << configs[i].listen_port << std::endl;
//             std::cout << "  Server Name: " << configs[i].server_name << std::endl;

//             // error_page の情報を表示する
//             if (!configs[i].error_pages.empty()) {
//                 std::cout << "  Error Pages:" << std::endl;
//                 for (std::map<int, std::string>::const_iterator it = configs[i].error_pages.begin();
//                      it != configs[i].error_pages.end(); ++it) {
//                     std::cout << "    Error Code " << it->first << " -> " << it->second << std::endl;
//                 }
//             } else {
//                 std::cout << "  Error Pages: None" << std::endl;
//             }

//             // location ブロックの情報を表示する
//             if (!configs[i].locations.empty()) {
//                 std::cout << "  Locations:" << std::endl;
//                 for (size_t j = 0; j < configs[i].locations.size(); ++j) {
//                     const LocationConfig& loc = configs[i].locations[j];
//                     std::cout << "    Location " << loc.getPath() << ":" << std::endl;
//                     std::cout << "      Root: " << loc.getRoot() << std::endl;
//                     std::cout << "      Index: " << loc.getIndex() << std::endl;
//                     std::cout << "      Autoindex: " << (loc.getAutoindex() ? "On" : "Off") << std::endl;

//                     // allowed_methods の情報を表示する
//                     const std::set<std::string>& methods = loc.getAllowedMethods();
//                     if (!methods.empty()) {
//                         std::cout << "      Allowed Methods: ";
//                         for (std::set<std::string>::const_iterator it = methods.begin(); it != methods.end(); ++it) {
//                             std::cout << *it << " ";
//                         }
//                         std::cout << std::endl;
//                     } else {
//                         std::cout << "      Allowed Methods: None" << std::endl;
//                     }
//                 }
//             } else {
//                 std::cout << "  Locations: None" << std::endl;
//             }
//         }
//     } else {
//         // 解析に失敗した場合のエラーメッセージ
//         std::cerr << "Failed to parse configuration." << std::endl;
//     }

//     return 0;
// }

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

            for (size_t j = 0; j < server.locations.size(); ++j)
            {
                const LocationConfig& loc = server.locations[j];
                std::cout << "  Location " << j + 1 << ":" << std::endl;
                std::cout << "    Path: " << loc.path << std::endl;
                std::cout << "    Root: " << loc.root << std::endl;
                std::cout << "    Index: " << loc.index << std::endl;
                std::cout << "    Allow Methods: ";
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

    return 0;
}