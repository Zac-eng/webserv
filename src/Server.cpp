#include <iostream>
#include <string>
#include <vector>
#include "Server.hpp"
#include "Request.hpp"

Server::Server(std::vector<ServerConfig>& conf) : _conf(conf) {};

bool Server::ListenSocketCreate(void)
{
	std::vector<Socket> socket_array;

	for (std::vector<ServerConfig>::iterator it = _conf.begin(); it != _conf.end(); it++)
	{
		Socket socket(*it);
		std::cout << "-------------" << std::endl;
		exit(0);
		if (socket.SocketCreate() == false)
			return (false);
		socket_array.push_back(socket);
	}
	this->_socket = socket_array;
	return (true);
}

bool Server::SetMonitoringFd(Socket& socket)
{
	struct epoll_event event;

	event.events = EPOLLIN;
	event.data.fd = socket._server_fd;
	if (epoll_ctl(this->_epoll_fd, EPOLL_CTL_ADD, socket._server_fd, &event) < 0)
	{
		// CloseEpollFd();
		return (false);
	}
	return (true);
}

bool Server::EpollCreate(void)
{
	this->_epoll_fd = epoll_create1(0);

	if (this->_epoll_fd < 0)
		return (false);
	for (std::vector<Socket>::iterator it = _socket.begin(); it != _socket.end(); it++)
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
	Socket socket;
	// struct sockaddr_in address;
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
		return (false);
	}
	// socket.SetListenFlag(false);
	return(true);
}

bool Server::CheckListenFd(int fd)
{
	for (std::vector<Socket>::iterator it = _socket.begin(); it != _socket.end(); it++)
	{
		// if (it->GetFd() == fd && it->GetListenFdFlag() == true)
			return (true);
	}
	return (false);
}


bool Server::ExecuteLoop()
{
	int event_counts;
	struct epoll_event events[MAX_EVENTS];

	while (true)
	{
		event_counts = epoll_wait(this->_epoll_fd, events, MAX_EVENTS, -1);
		for (int i = 0; i < event_counts; i++)
		{
			if (CheckListenFd(events[i].data.fd) == true)
			{
				Client client;
				//client
				if (SetConnectFd(events[i].data.fd) == false)
					return (false);
				//clientfdを作成する。
			}
			// else
			// 	_client.AcceptRequest();
		}
	}
}