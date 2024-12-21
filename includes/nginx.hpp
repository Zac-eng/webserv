#ifndef NGINX_HPP
#define NGINX_HPP

#include <string>
#include <vector>
#include <stdexcept>
#include <functional>
#include <map>
#include <stdlib.h>
#include <iostream>


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


template <typename T>
void debug1(const T& value)
{
	std::cout << "デバック: " << value << std::endl;
	exit(0);
}

template <typename T>
void debug2(T& value)
{
	typename T::iterator it = value.begin();
	std::cout << "デバック" <<std::endl; 
	for (; it != value.end(); it++)
		std::cout << *it << std::endl;
	exit(0);
}




#endif