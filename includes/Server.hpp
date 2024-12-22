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
#include "Socket.hpp"
#include "Client.hpp"
#include "nginx.hpp"
#include <stdlib.h>
#include <cstring>

#define MAX_EVENTS 10

class Server {
	// private:
		std::map<int, Socket> _socket; //_listen,fd作成
		std::vector<ServerConfig> _conf;
		std::map<int, Client> _client; //clientのコネクト
		int _epoll_fd;
		bool _listen_fd;
		

	public:
		Server(std::vector<ServerConfig>& _conf);
		void ServerRequest();
		void MethodWait(std::map<int, std::string>& array_buffer);
		void AcceptRequest(std::map<int, std::string>& array_buffer, int i);
		void BindSocket(int server_fd);
		int SetSocket(void);
		void ServerCreateetNonBlock(int fd);
bool ListenSocketCreate(void);
bool SetMonitoringFd(Socket& socket);
bool EpollCreate(void);
bool ServerCreate(void);
bool ExecuteLoop();
bool CheckListenFd(int fd);
bool SetConnectFd(int listen_fd);

void debug_server();
};



namespace Error
{
	void MissingRequestLineAndHost(void);
	void InvalidMethod(void);
	void InvalidUri(void);
	void InvalidVersion(void);
	void InvalidRequestLine(void);
	void InvalidHeaderKey(void);
	void InvalidHeaderValue(void);
	void ReadingBuffer(void);
}

bool SetNonBlocking(int fd);
