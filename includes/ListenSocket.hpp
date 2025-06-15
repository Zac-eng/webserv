#pragma once

#include "ASocket.hpp"

class ListenSocket : public ASocket
{
	public:
	//Listen fdの作成
	bool createSocket();
	// Client fdの作成
	void handleEpollInEvent(int epoll_fd, std::map<int, ASocket*>& _socket);
	// 何もなし
	void handleEpollOutEvent(int epoll_fd, std::map<int, ASocket*>& _socket);
	bool handleTimeOut(int epoll_fd, std::map<int, ASocket*>& _socket, int fd);


	ListenSocket();
	ListenSocket(ServerConfig& conf);
	~ListenSocket();
	bool socketInit(void);
	bool setSocket(void);
	bool bindSocket();
	bool createListenSocket();
	bool socketCreate(void);
	ServerConfig getConf() const;

};