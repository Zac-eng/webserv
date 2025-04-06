#pragma once

#include "location.hpp"
#include "nginx.hpp"
#include <iostream>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include "nginx.hpp"
#include <cstring>
#include <arpa/inet.h>

#define BUFFER_SIZE 100

class ASocket
{
	public:
	ServerConfig _conf;
	int _fd;
	std::string _host_name;
	int _port;

	ASocket();
	~ASocket();
	int GetFd(void) const;
	void SetFd(int fd);
	// virtual bool CreateSocket() = 0;
	virtual bool handleEpollInEvent(int epoll_fd, std::map<int, ASocket*>& _socket) = 0;
	virtual void handleEpollOutEvent() = 0;
};
