#pragma once

#include "nginx.hpp"
#include "ASocket.hpp"
#include <fcntl.h>
#include <sys/epoll.h>
#include <cstdlib>
#include "ListenSocket.hpp"
#include <unistd.h>

#define MAX_EVENTS 10

class Server
{
	public:
	std::map<int, ASocket*> _socket;
	std::vector<ServerConfig> _conf;
	int _epoll_fd;

	Server();
	void createListenServer(void);
	void executeServer(void);
	Server(std::vector<ServerConfig>& conf);
	bool epollCreate(void);
	bool setMonitoringFd(ASocket* socket);
	void closeFdAndFree(void);

	class ServerException : public std::exception {
		const char* what() const throw();
	};

};

bool set_nonblocking(int fd);
