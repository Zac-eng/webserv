#pragma once

#include <sys/socket.h>

class Socket
{
	private:
		ServerConfig _conf;
		int _server_fd;
		int _port;
		int _max_connect;
		std::string _host_name;

	public:
		Socket();
		Socket(ServerConfig& conf);
		~Socket();
}