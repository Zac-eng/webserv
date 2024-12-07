#ifndef NGINX_HPP
#define NGINX_HPP

#include <string>
#include <vector>
#include <stdexcept>
#include <functional>


class ServerConfig
{
	public:
		//ポート番号
		int listen_port;
		//サーバー名
		std::string server_name;

		// std::vector<LocationConfig> locations;

		ServerConfig() : listen_port(0) {}
		//材料チェック

		void validate() const;
};

bool	parse_config(const std::string& filename, std::vector<ServerConfig>& configs);
std::string trim(const std::string& str);
std::string	extract_quoted_string(const std::string& str);


#endif