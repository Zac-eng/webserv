#pragma once

#include <iostream>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <poll.h>

#define PORT 8080
#define BACKLOG 5
#define MAX_SIZE 1024

class Server {
	private:
		int _server_fd;
		struct pollfd _poll_fds[BACKLOG];
		

	public:
		Server();
		void ServerRequest();
};