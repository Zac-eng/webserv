#pragma once

#include "location.hpp"
#include "nginx.hpp"
#include <iostream>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>
#include <arpa/inet.h>
#include <sys/epoll.h>

#define BUFFER_SIZE 2048

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
		virtual ~ASocket();
		int getFd(void) const;
		void setFd(int fd);
		ServerConfig getConf(void) const;
		void setConf(ServerConfig conf);
		std::string getHostName(void) const;
		void setHostName(std::string& host_name);
		int getPort(void) const;
		void setPort(int port);
		virtual bool createSocket() = 0;
		virtual void handleEpollInEvent(int epoll_fd, std::map<int, ASocket*>& _socket) = 0;
		virtual void handleEpollOutEvent(int epoll_fd, std::map<int, ASocket*>& _socket) = 0;
};
