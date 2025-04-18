#pragma once

#include "location.hpp"
#include "nginx.hpp"
#include <iostream>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>
#include <arpa/inet.h>

class ASocket
{
	
public:
	ServerConfig _conf;
	int _fd;
	std::string _host_name;
	int _port;

	ASocket();
	ASocket(ServerConfig& conf);
	~ASocket();
	int GetFd(void) const;
	void SetFd(int fd);
	virtual bool createSocket() = 0;
	virtual bool handleEpollInEvent() = 0;
	virtual void handleEpollOutEvent() = 0;
};
