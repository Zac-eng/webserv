#pragma once

#include "nginx.hpp"
#include "ASocket.hpp"
#include <fcntl.h>
#include <sys/epoll.h>
#include <cstdlib>
#include "ListenSocket.hpp"
#include <unistd.h>
#include "ServerException.hpp"

#define MAX_EVENTS 10
#define EPOLL_TIME 10
#define TIMEOUT 20


class Server
{
	public:
	std::map<int, ASocket*> _socket;
	std::vector<ServerConfig> _conf;
	int _epoll_fd;
	

	Server();
	~Server();
	void createListenServer(void);
	void executeServer(void);
	Server(std::vector<ServerConfig>& conf);
	bool epollCreate(void);
	bool setMonitoringFd(ASocket* socket);
	void checkTimeOut();

	std::vector<ServerConfig> getConf(void);
	void setConf(std::vector<ServerConfig>& conf);
};

bool set_nonblocking(int fd);