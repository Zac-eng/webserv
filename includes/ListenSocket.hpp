#pragma once

#include "ASocket.hpp"

class ListenSocket : public ASocket
{
	public:
	//Listen fdの作成
	bool createSocket();
	// Client fdの作成
	bool handleEpollInEvent();
	// 何もなし
	void handleEpollOutEvent();


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