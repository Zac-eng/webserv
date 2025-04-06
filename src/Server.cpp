#include "Server.hpp"

Server::Server(std::vector<ServerConfig>& conf) : _conf(conf) {};

bool set_non_blocking(int fd)
{
	int flag;

	flag = fcntl(fd, F_GETFL);
	if (flag == -1)
		throw std::runtime_error("NonBlocking Fd");
	// 現在の性能にNON_BLOCKを追加。3BIT目がNON_BLOCKのフラグ。
	flag |= O_NONBLOCK;
	if (fcntl(fd, F_SETFL, flag) == -1)
		throw std::runtime_error("NonBlocking Fd");
	return (true);
}

// todo、falseが返ってきた時にsocketを全てfreeして終了。
void Server::CreateListenServer(std::vector<ServerConfig>& conf)
{
	std::vector<ServerConfig>::iterator it;

	for (; conf.end() != it; it++)
	{
		ASocket *socket = new ListenSocket();
		if (socket->CreateSocket(*it) == false)
			return ;
		if (SetNonBlocking(socket._server_fd) ==  false)
			return ;
		this->_socket.insert(std::make_pair(socket->GetFd(), socket));
	}
}

void Server::CloseEpollFd(void)
{
	for (std::map<int, Socket>::iterator it = this->_socket.begin(); it != this->_socket.end(); it++)
		close(it->first);
	for (std::map<int, Client>::iterator it = this->_client.begin(); it != this->_client.end(); it++)
		close(it->first);
}


void Server::ExecuteServer(void)
{
	int event_counts;
	struct epoll_event event[MAX_EVENTS];

	while (true)
	{
		event_counts = epoll_wait(this->_epoll_fd, event, MAX_EVENTS + 1, -1);
		if (event_counts == -1)
			throw std::runtime_error("epoll_wait");
		for (int i = 0; i < event_counts; i++)
		{
			if (event[i].events == EPOLLIN)
			{
				if (this->_socket[event[i].data.fd].HandleEpollInEvent(this->_epoll_fd, this->_socket) == false)
					return (false);
			}
			else if (event[i].events == EPOLLOUT)
			{
				if (this->_socket[event[i].data.fd].HandleEpollOutEvent() == false)
					return (false);
			}
		}
	}
}
