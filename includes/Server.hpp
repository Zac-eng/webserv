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

class Server
{
public:
	static int _epoll_fd;
	static std::map<int, ASocket*> _socket;
	std::vector<ServerConfig> _conf;

	Server();
	~Server();
	void createListenServer(void);
	void executeServer(void);
	Server(std::vector<ServerConfig>& conf);
	bool epollCreate(void);
	bool setMonitoringFd(ASocket* socket);
};

bool set_nonblocking(int fd);
