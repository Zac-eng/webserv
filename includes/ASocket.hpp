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

class ASocket
{
	private:
		ServerConfig _conf;
		int _fd;
		std::string _host_name;
		int _port;

	public:
		ASocket();
		ASocket(ServerConfig& conf);
		~ASocket();
		int getFd(void) const;
		void setFd(int fd);
		ServerConfig getConf(void) const;
		void ASocket::setConf(ServerConfig conf);
std::string ASocket::getHostName(void) const;
void ASocket::setHostName(std::string& host_name);

		virtual bool createSocket() = 0;
		virtual bool handleEpollInEvent(int epoll_fd, std::map<int, ASocket*>& _socket) = 0;
		virtual bool handleEpollOutEvent(int epoll_fd, std::map<int, ASocket*>& _socket) = 0;
};
