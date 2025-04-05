#pragma once

class Server
{
	public:
	std::map<int, ASocket*> _socket;
	std::vector<ServerConfig> _conf;
	int _epoll_fd;
	Response _response;

void CreateListenServer(std::vector<ServerConfig>& conf);
void ExecuteServer(void);
void CloseEpollFd(void);
};

bool SetNonBlocking(int fd);