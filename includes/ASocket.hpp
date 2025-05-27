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
		time_t _start_time;
		std::string _host_name;
		int _port;
		bool _time_out_flag;

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
		void setStartTime(time_t start_time);
		time_t getStartTime(void);

		bool getTimeOut(void) const;
		void setTimeOut(bool time_out_flag);
		
		virtual bool createSocket() = 0;
		virtual void handleEpollInEvent(int epoll_fd, std::map<int, ASocket*>& _socket) = 0;
		virtual void handleEpollOutEvent(int epoll_fd, std::map<int, ASocket*>& _socket) = 0;
};
