#include <iostream>
#include <string>
#include <vector>
#include "Server.hpp"
#include "Request.hpp"

Server::Server(std::vector<ServerConfig>& conf) : _conf(conf) {};

bool Server::ListenSocketCreate(void)
{
	this->_socket.clear();
	for (std::vector<ServerConfig>::iterator it = _conf.begin(); it != _conf.end(); it++)
	{
		Socket socket(*it);
		if (socket.SocketCreate() == false)
			return (false);
		this->_socket.push_back(socket);
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
	for (std::vector<Socket>::iterator it = this->_socket.begin(); it != this->_socket.end(); it++)
	{
		if (SetMonitoringFd(*it) ==  false)
			return (false);
	}
	return (true);
}

bool Server::ServerCreate(void)
{
	if (ListenSocketCreate() == false)
		return (false);
	if (EpollCreate() == false)
		return (false);
	return (true);
}

bool Server::SetConnectFd(int listen_fd)
{
	Socket socket(false);
	struct sockaddr_in address;
	socklen_t len = sizeof(address);
	struct epoll_event event;

	memset(&address, 0 ,len);
	socket._server_fd = accept(listen_fd, (struct sockaddr *)&address, &len);
	if (socket._server_fd < 0)
		return (false);
	event.events = EPOLLIN;
	event.data.fd = socket._server_fd;

	if (epoll_ctl(this->_epoll_fd, EPOLL_CTL_ADD, socket._server_fd, &event) < 0)
	{
		// CloseEpollFd();
		std::cout << "error" <<std::endl;
		return (false);
	}
	// socket.SetListenFlag(false);
	return(true);
}

bool Server::CheckListenFd(int fd)
{
	for (std::vector<Socket>::iterator it = _socket.begin(); it != _socket.end(); it++)
	{
		if (it->_server_fd == fd && it->_listen_flag == true)
			return (true);
	}
	return (false);
}


bool Server::ExecuteLoop()
{
	int event_counts;
	struct epoll_event event[MAX_EVENTS];

	while (true)
	{
		event_counts = epoll_wait(this->_epoll_fd, event, MAX_EVENTS, -1);
		for (int i = 0; i < event_counts; i++)
		{
			if (CheckListenFd(event[i].data.fd) == true && event[i].events & EPOLLIN)
			{
				Client client;
				//client

				if (SetConnectFd(event[i].data.fd) == false)
					return (false);
				this->_client[event[i].data.fd] = client;
				//clientfdを作成する。
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