#ifndef NGINX_HPP
#define NGINX_HPP

#include <string>
#include <vector>
#include <stdexcept>
#include <functional>
#include <map>


class ServerConfig
{
	public:
		//ポート番号
		int _listen_port;
		//サーバー名
		std::string _server_name;

		ServerConfig(int port, std::string server);
		ServerConfig();
		~ServerConfig();
};



#endif