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
#include <sys/epoll.h>

#define MAX_EVENTS 10

class Server {
	private:
		std::vector<Socket> _socket;
		std::vector<ServerConfig> _conf;
		std::vector<Client> _client;
		int _poll_fd;
		bool _listen_fd;
		

	public:
		Server(ServerConf& _conf);
		void ServerRequest();
		void MethodWait(std::map<int, std::string>& array_buffer);
		void AcceptRequest(std::map<int, std::string>& array_buffer, int i);
		void BindSocket(int server_fd);
		int SetSocket(void);
		void	SetNonBlock(int fd);
};