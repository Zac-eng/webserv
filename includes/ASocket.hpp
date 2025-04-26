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
	protected:
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
		void setConf(ServerConfig conf);
		std::string getHostName(void) const;
		void setHostName(std::string& host_name);
		int getPort(void) const;
		void setPort(int port);
		virtual bool createSocket() = 0;
		virtual bool handleEpollInEvent(int epoll_fd, std::map<int, ASocket*>& _socket) = 0;
		virtual bool handleEpollOutEvent(int epoll_fd, std::map<int, ASocket*>& _socket) = 0;
};
