#include <iostream>
#include <string>
#include <vector>
#include "Server.hpp"
#include "Request.hpp"

Server::Server(std::vector<ServerConfig>& conf) : _conf(conf) {};

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

bool Server::ListenSocketCreate(void)
{
	this->_socket.clear();
	for (std::vector<ServerConfig>::iterator it = _conf.begin(); it != _conf.end(); it++)
	{
		Socket socket(*it);
		if (socket.SocketCreate() == false)
			return (false);
		if (SetNonBlocking(socket._server_fd) ==  false)
			return (false);
		this->_socket[socket._server_fd] = socket;
	}
	return (true);
}

bool Server::SetMonitoringFd(Socket& socket)
{
	struct epoll_event event;

	event.events = EPOLLIN;
	event.data.fd = socket._server_fd;
	if (epoll_ctl(this->_epoll_fd, EPOLL_CTL_ADD, socket._server_fd, &event) < 0)
	{
		std::cout << "error" <<std::endl;
		return (false);
	}
	   std::cout << "Listening socket added to epoll: " << socket._server_fd << std::endl;
	return (true);
}

bool Server::EpollCreate(void)
{
	this->_epoll_fd = epoll_create1(0);

	if (this->_epoll_fd < 0)
		return (false);
	for (std::map<int, Socket>::iterator it = this->_socket.begin(); it != this->_socket.end(); it++)
	{
		if (SetMonitoringFd(it->second) ==  false)
			return (false);
	}
	return (true);
}

bool Server::ServerCreate()
{
	if (ListenSocketCreate() == false)
		return (false);
	if (EpollCreate() == false)
		return (false);
	return (true);
}

void Server::CloseEpollFd(void)
{
	for (std::map<int, Socket>::iterator it = this->_socket.begin(); it != this->_socket.end(); it++)
		close(it->first);
	for (std::map<int, Client>::iterator it = this->_client.begin(); it != this->_client.end(); it++)
		close(it->first);
}

bool Server::SetConnectFd(int listen_fd)
{
	int fd;
	Client client(this->_socket[listen_fd].GetConf());
	struct sockaddr_in address;
	socklen_t len = sizeof(address);
	struct epoll_event event;

	memset(&address, 0 ,len);
	fd = accept(listen_fd, (struct sockaddr *)&address, &len);
	if (fd < 0)
		return (false);
	event.events = EPOLLIN;
	event.data.fd = fd;

	if (epoll_ctl(this->_epoll_fd, EPOLL_CTL_ADD, fd, &event) < 0)
		throw std::runtime_error("epoll_ctl");
	this->_client[fd] = client;
	if (SetNonBlocking(fd) ==  false)
		return (false);
	this->_client[fd]._fd = fd;
	return(true);
}

bool Server::CheckListenFd(int fd)
{
	if (this->_socket.count(fd) == 1 && this->_socket[fd]._listen_flag == true)
		return (true);
	return (false);
}


bool Server::ExecuteLoop()
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
			if (CheckListenFd(event[i].data.fd) == true && event[i].events & EPOLLIN)
			{
				if (SetConnectFd(event[i].data.fd) == false)
					return (false);
			}
			else if (event[i].events == EPOLLIN)
			{
				if (_client[event[i].data.fd].AcceptRequest() == false)
					return (false);
			}
		}
	}
}

void Server::debug_server()
{
	std::cout << _epoll_fd << std::endl;
	exit(0);
}
