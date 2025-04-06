#include "Server.hpp"

Server::Server(std::vector<ServerConfig>& conf) : _conf(conf) {};

Server::Server() {};

bool SetNonBlocking(int fd)
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

bool Server::SetMonitoringFd(ASocket* socket)
{
	struct epoll_event event;

	event.events = EPOLLIN;
	event.data.fd = socket->_fd;
	if (epoll_ctl(this->_epoll_fd, EPOLL_CTL_ADD, socket->_fd, &event) < 0)
	{
		std::cout << "error" <<std::endl;
		return (false);
	}
	   std::cout << "Listening socket added to epoll: " << socket->_fd << std::endl;
	return (true);
}

bool Server::EpollCreate(void)
{
	this->_epoll_fd = epoll_create1(0);

	if (this->_epoll_fd < 0)
		return (false);
	for (std::map<int, ASocket*>::iterator it = this->_socket.begin(); it != this->_socket.end(); it++)
	{
		if (SetMonitoringFd(it->second) ==  false)
			return (false);
	}
	return (true);
}

// todo、falseが返ってきた時にsocketを全てfreeして終了。
void Server::CreateListenServer(void)
{
	std::vector<ServerConfig>::iterator it;

	it = this->_conf.begin();
	for (; this->_conf.end() != it; it++)
	{
		ASocket *socket = new ListenSocket(*it);
		if (socket->CreateSocket() == false)
			return ;
		if (SetNonBlocking(socket->_fd) ==  false)
			return ;
		this->_socket.insert(std::make_pair(socket->GetFd(), socket));
	}
	if (EpollCreate() == false)
		return ;
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
				if (this->_socket[event[i].data.fd]->HandleEpollInEvent(this->_epoll_fd, this->_socket) == false)
					return ;
			}
			else if (event[i].events == EPOLLOUT)
			{
				this->_socket[event[i].data.fd]->HandleEpollOutEvent();
					return ;
			}
		}
	}
}
