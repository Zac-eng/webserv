#pragma once

#include <iostream>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include "nginx.hpp"
#include <cstring>
#include <arpa/inet.h>

class Socket
{
	// private:
	public:
		ServerConfig _conf;
		int _server_fd;
		int _port;
		int _max_connect;
		std::string _host_name;

		Socket();
		Socket(ServerConfig& conf);
		~Socket();
bool SocketInit(void);
bool SetSocket(void);
bool BindSocket();
bool ListenSocket();
bool SocketCreate(void);

};