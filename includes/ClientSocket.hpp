#ifndef CLIENTSOCKET_HPP
#define CLIENTSOCKET_HPP


#include "location.hpp"
#include "nginx.hpp"
#include "Request.hpp"
#include "Response.hpp"
#include "ASocket.hpp"
#include <iostream>
#include <string>
#include <sys/types.h>
#include <vector>
#include <utility>
#include <fstream>
#include <sstream>
#include <map>
#include <sys/epoll.h>

#define BUFFER_SIZE 100

class ASocket;

class ClientSocket : public ASocket
{
	public:
	Request _request;
	Response _response;
	ServerConfig _conf;
	bool _carrige_return_flag;
	bool _response_flag;
	bool _first_post_flag;
	bool _complete_post_flag;
	bool _complete_parse_flag;
	bool _post_body_flag;
	std::string _buffer;

	ClientSocket();
	~ClientSocket();
	ClientSocket(ServerConfig& conf);
	// 何もない
	bool CreateSocket();
	// Requestパース
	bool HandleEpollInEvent(int epoll_fd, std::map<int, ASocket*>& _socket);
	// CgiSocket作成、レスポンス作成
	void HandleEpollOutEvent();
	bool ValidRequest(std::string& buffer, std::string::iterator& it, std::string& object);
	void ReadClientFd();
	void ParseLocation();
	bool CheckExecuteResponse(int epoll_fd);
bool CheckPostFlag();
bool CloseClientFd();
bool CheckCRequestFlag(std::string& buffer, std::string::iterator& it, std::string& object);
void ValidLocation(LocationConfig& location, LocationConfig& location_tmp, bool& location_flag);
bool CheckAndChangeLocationUri(std::vector<LocationConfig>& location, const std::string& uri);
void CombineUriAndLocationRoot(LocationConfig& location);
void ChangeDefaultPath(const std::string& uri);
void ChangeConfUri(const std::string& uri);
bool CheckAndChangeRootUri(const std::string& uri);

};
bool CompareLocationAndUri(const std::string& new_location, const std::string& before_location);
bool LoopCheckPath(std::string& uri, std::string& location_uri, bool& location_flag);

bool SkipSpaceAndCheckEnd(const std::string& request, std::string::const_iterator& it);
bool GetSubstringUntilSpace(const std::string& request, std::string::const_iterator& it, std::string& object);
bool GetSubstringUntilCarriageReturn(const std::string& request, std::string::const_iterator& it, std::string& object);
bool isCarriagereturn(const std::string& request);
bool SubstringObject(std::string& buffer, std::string::iterator& it,std::string& object);

#endif