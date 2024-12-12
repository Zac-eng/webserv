#pragma once

#include <iostream>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <poll.h>
#include <fstream>
#include <sstream>
#include <map>
#include <fcntl.h>
#include <unistd.h>


#define PORT 8080
#define BACKLOG 5
#define MAX_SIZE 1024

class Server {
	private:
		int _server_fd;
		struct pollfd _poll_fds[BACKLOG];
		std::map<int, std::string> _request_buffer;
		

	public:
		Server();
		void ServerRequest();
		void MethodWait(std::map<int, std::string>& array_buffer);
		void AcceptRequest(std::map<int, std::string>& array_buffer, int i);
		void BindSocket(int server_fd);
		int SetSocket(void);
		void	SetNonBlock(int fd);
};