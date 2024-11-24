#include <iostream>
#include <string>
#include <vector>
#include "Server.hpp"
#include "Request.hpp"

int Server::SetSocket(void)
{
	int server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (server_fd == -1)
	{
		throw std::runtime_error("socket failed");
	}
	int opt = 1;
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
	{
		throw std::runtime_error("set socket failed");
	}
	if (setsockopt(server_fd, SOL_SOCKET,SO_REUSEPORT, &opt, sizeof(opt)) < 0)
	{
		throw std::runtime_error("set socket failed");
	}
	return (server_fd);
}

void Server::BindSocket(int server_fd)
{
	struct sockaddr_in address;

	memset(&address, 0, sizeof(address));
	address.sin_family = AF_INET;
	address.sin_port = htons(8080);
	address.sin_addr.s_addr = htonl(INADDR_ANY);
	if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
	{
		throw std::runtime_error("bind failed");
	}
	return ;
}

void Server::SetNonBlock(int fd)
{
	int flag;

	flag = fcntl(fd, F_GETFL, 0);
	if (flag == -1)
	{
		close(fd);
		throw std::runtime_error("fcntl failed");
	}
	if (fcntl(fd, F_SETFL, flag | O_NONBLOCK) == -1)
	{
		close(fd);
		throw std::runtime_error("fcntl failed");
	}
}

Server::Server()
{
	int server_fd = 0;

	std::cout << "Server" << std::endl;
	server_fd = this->SetSocket();
	this->BindSocket(server_fd);
	this->_poll_fds[0].fd = server_fd;
	this->_poll_fds[0].events = POLLIN;
	this->_server_fd = server_fd;
	for (int i = 1; i < BACKLOG; i++)
		this->_poll_fds[i].fd = -1;
	if (listen(server_fd, BACKLOG) < 0)
		throw std::runtime_error("listen failed");
	SetNonBlock(this->_poll_fds[0].fd);
}

void	Server::AcceptRequest(std::map<int, std::string>& array_buffer, int i)
{
	char buffer[MAX_SIZE];
	int bytes_read;

	memset(buffer, 0 , sizeof(buffer));
	Request req(this->_poll_fds[i].fd, "index.html");
	while (true)
	{
		bytes_read = read(this->_poll_fds[i].fd, buffer, sizeof(buffer) - 1);
		std::cout<< "buffer :" << buffer <<std::endl;
		if (bytes_read > 0)
		{
			buffer[bytes_read] = '\0';
			if (array_buffer.count(this->_poll_fds[i].fd))
				array_buffer[this->_poll_fds[i].fd] += buffer;
			else
				array_buffer.insert(std::make_pair(_poll_fds[i].fd, buffer));
		}
		if (bytes_read < 0)
		{
			array_buffer.erase(this->_poll_fds[i].fd);
			close(this->_poll_fds[i].fd);
			this->_poll_fds[i].fd = -1;
			this->_poll_fds[i].revents = 0;
			std::cout << "Client disconnected: socket fd " << this->_poll_fds[i].fd << std::endl;
			break;
		}
		else
		{
			buffer[bytes_read] = '\0';
			std::cout << "Client fd " << this->_poll_fds[i].fd << ": message: " << buffer << std::endl;
			if (req.HandleMethod(buffer) == false)
			{
				array_buffer.erase(this->_poll_fds[i].fd);
				close(this->_poll_fds[i].fd);
				this->_poll_fds[i].fd = -1;
				this->_poll_fds[i].revents = 0;
				return ;
			}
			array_buffer.erase(this->_poll_fds[i].fd);
		}
	}
}

void	Server::MethodWait(std::map<int, std::string>& array_buffer)
{
	for (int i = 1; i < BACKLOG; i++)
	{
		std::cout << "--------Client fd " << this->_poll_fds[i].fd << ": data received" << std::endl;
		if (this->_poll_fds[i].fd != -1 && (this->_poll_fds[i].revents & POLLIN))
		{
			std::cout << "Client fd " << this->_poll_fds[i].fd << ": data received" << std::endl;
			AcceptRequest(array_buffer, i);
		}
	}
}

void	Server::ServerRequest()
{
	int client_fd;
	int poll_fd;
	struct sockaddr_in address;
	socklen_t len = sizeof(address);
	std::map<int, std::string> array_buffer;

	memset(&address, 0, sizeof(address));
	std::cout << "ServerRequest" << std::endl;
	while (1)
	{
		std::cout << "メッセージを受け取る待機前" << std::endl;
		poll_fd = poll(this->_poll_fds, BACKLOG, -1);
		std::cout << "メッセージを受け取る待機後です。" << std::endl;
		if (poll_fd < 0)
		{
			throw std::runtime_error("poll failed");
		}
		if (this->_poll_fds[0].revents & POLLIN)
		{
			std::cout << "New client connected" << std::endl;
			client_fd = accept(_server_fd, (struct sockaddr *)&address, &len);
			std::cout << "New client connected: socket fd " << client_fd << std::endl;
			if (client_fd < 0)
			{
				std::cout << "accept failed" << std::endl;
				continue;
			}
			SetNonBlock(client_fd);
			for (int i = 1; i < BACKLOG; i++)
			{
				if (this->_poll_fds[i].fd == -1)
				{
					this->_poll_fds[i].fd = client_fd;
					this->_poll_fds[i].events = POLLIN;
					// default_page(client_fd);
					break;
				}
			}
		}
		MethodWait(array_buffer);
	}
}