#pragma once

#include "ASocket.hpp"

class ListenSocket : public ASocket
{
	public:
	//Listen fdの作成
	bool CreateSocket();
	// Client fdの作成
	bool HandleEpollInEvent(int epoll_fd, std::map<int, ASocket*>& _socket);
	// 何もなし
	void HandleEpollOutEvent();


ListenSocket();
ListenSocket(ServerConfig& conf);
~ListenSocket();
bool SocketInit(void);
bool SetSocket(void);
bool BindSocket();
bool CreateListenSocket();
bool SocketCreate(void);
ServerConfig GetConf();
};