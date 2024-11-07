#include <iostream>
#include <string>
#include <vector>
#include "Server.hpp"

Server::Server()
{
	struct sockaddr_in address;
	std::cout << "Server" << std::endl;
	int server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (server_fd == -1)
	{
		throw std::runtime_error("socket failed");
	}
	memset(&address, 0, sizeof(address));
	address.sin_family = AF_INET;
	address.sin_port = htons(8080);
	address.sin_addr.s_addr = htonl(INADDR_ANY);
	if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
	{
		throw std::runtime_error("bind failed");
	}
	this->_poll_fds[0].fd = server_fd;
	this->_poll_fds[0].events = POLLIN;
	_server_fd = server_fd;
	for (int i = 1; i < BACKLOG; i++)
	{
		this->_poll_fds[i].fd = -1;
	}
	if (listen(server_fd, BACKLOG) < 0)
	{
		throw std::runtime_error("listen failed");
	}
}

void	Server::ServerRequest()
{
	int client_fd;
	struct sockaddr_in address;
	socklen_t len = sizeof(address);
	memset(&address, 0, sizeof(address));
	std::cout << "ServerRequest" << std::endl;
	while (1)
	{
		int poll_fd = poll(this->_poll_fds, BACKLOG, -1);
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
			for (int i = 1; i < BACKLOG; i++)
			{
				if (this->_poll_fds[i].fd == -1)
				{
					this->_poll_fds[i].fd = client_fd;
					this->_poll_fds[i].events = POLLIN;
					break;
				}
			}
		}
			for (int i = 1; i < BACKLOG; i++)
			{
				std::cout << "--------Client fd " << this->_poll_fds[i].fd << ": data received" << std::endl;
				if (this->_poll_fds[i].fd != -1 && (this->_poll_fds[i].revents & POLLIN))
				{
					std::cout << "New client connected: socket fd set" << client_fd << std::endl;
					std::cout << "Client fd " << this->_poll_fds[i].fd << ": data received" << std::endl;
					char buffer[MAX_SIZE];
					int bytes_read = read(this->_poll_fds[i].fd, buffer, sizeof(buffer) - 1);
					if (bytes_read <= 0)
					{
						close(this->_poll_fds[i].fd);
						this->_poll_fds[i].fd = -1;
						std::cout << "Client disconnected: socket fd " << this->_poll_fds[i].fd << std::endl;
					}
					else
					{
						buffer[bytes_read] = '\0';
						std::cout << "Client fd " << this->_poll_fds[i].fd << ": message: " << buffer << std::endl;
						send(this->_poll_fds[i].fd, buffer, strlen(buffer), 0);
					}
				}
		}
	}
}

int main()
{
	// if (ac != 2) {
	// 	std::cerr << "Invalid arguments" << std::endl;
	// 	return 1;
	// }
	try
	{
		// std::string message = av[1];
		// Class nginx(message);
		// nginx_parse(nginx);
		Server server;
		server.ServerRequest();

	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() << std::endl;
		return 1;
	}
}