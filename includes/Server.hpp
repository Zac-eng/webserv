#pragma once

#include "nginx.hpp"
#include "ASocket.hpp"
#include <fcntl.h>
#include <sys/epoll.h>
#include <cstdlib>
#include "ListenSocket.hpp"

#define MAX_EVENTS 10

class Server
{
	public:
	std::map<int, ASocket*> _socket;
	std::vector<ServerConfig> _conf;
	int _epoll_fd;

Server();
void CreateListenServer(void);
void ExecuteServer(void);
Server(std::vector<ServerConfig>& conf);
bool EpollCreate(void);
bool SetMonitoringFd(ASocket* socket);
};

bool SetNonBlocking(int fd);