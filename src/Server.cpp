#include "Server.hpp"

Server::Server(std::vector<ServerConfig>& conf) : _conf(conf) {};

Server::Server() {};

Server::~Server()
{
	std::map<int, ASocket*>::iterator it;

	it = this->_socket.begin();
	for (; it != this->_socket.end(); it++)
	{
		close(it->first);
		delete it->second;
	}
}


bool set_nonblocking(int fd)
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

bool Server::setMonitoringFd(ASocket* socket)
{
	struct epoll_event event;

	event.events = EPOLLIN;
	event.data.fd = socket->getFd();
	if (epoll_ctl(this->_epoll_fd, EPOLL_CTL_ADD, socket->getFd(), &event) < 0)
		return (false);
	return (true);
}

bool Server::epollCreate(void)
{
	this->_epoll_fd = epoll_create1(0);

	if (this->_epoll_fd < 0)
		return (false);
	for (std::map<int, ASocket*>::iterator it = this->_socket.begin(); it != this->_socket.end(); it++)
	{
		if (setMonitoringFd(it->second) ==  false)
			return (false);
	}
	return (true);
}

// todo、falseが返ってきた時にsocketを全てfreeして終了。
void Server::createListenServer(void)
{
	std::vector<ServerConfig>::iterator it;

	it = this->_conf.begin();
	for (; this->_conf.end() != it; it++)
	{
		ASocket *socket = new ListenSocket(*it);
		if (socket->createSocket() == false)
			throw ServerException();
		if (set_nonblocking(socket->getFd()) == false)
			throw ServerException();
		this->_socket.insert(std::make_pair(socket->getFd(), socket));
	}
	if (epollCreate() == false)
		throw ServerException();
}

void Server::executeServer(void)
{
	int event_counts;
	struct epoll_event event[MAX_EVENTS];

	while (true)
	{
		event_counts = epoll_wait(this->_epoll_fd, event, MAX_EVENTS + 1, -1);
		if (event_counts == -1)
			throw ServerException();
		for (int i = 0; i < event_counts; i++)
		{
			if (event[i].events == EPOLLIN)
			{
				if (this->_socket[event[i].data.fd]->handleEpollInEvent(this->_epoll_fd, this->_socket) == false)
					std::cout <<"error" <<std::endl;
			}
			else if (event[i].events == EPOLLOUT)
			{
				if (this->_socket[event[i].data.fd]->handleEpollOutEvent(this->_epoll_fd, this->_socket) == false)
					std::cout <<"error" <<std::endl;
			}
		}
	}
}
