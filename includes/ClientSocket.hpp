#ifndef CLIENTSOCKET_HPP
#define CLIENTSOCKET_HPP


#include "location.hpp"
#include "nginx.hpp"
#include "Request.hpp"
#include "Response.hpp"
#include "ASocket.hpp"
#include "Server.hpp"
#include <iostream>
#include <string>
#include <sys/types.h>
#include <vector>
#include <utility>
#include <fstream>
#include <sstream>
#include <map>

class ASocket;

class ClientSocket : public ASocket
{
	public:
	Request _request;
	Response _response;
	ServerConfig _conf;
	bool _carrige_return_flag;
	bool _response_flag;
	bool _complete_post_flag;
	bool _complete_parse_flag;
	bool _post_body_flag;
	std::string _buffer;
	sockaddr_in _client_addr;

	ClientSocket();
	~ClientSocket();
	ClientSocket(ServerConfig& conf, const sockaddr_in& addr);
	// 何もない
	bool createSocket();
	// Requestパース
	bool handleEpollInEvent();
	// CgiSocket作成、レスポンス作成
	bool handleEpollOutEvent();
	bool validRequest(std::string& buffer, std::string::iterator& it, std::string& object);
	bool checkExecuteResponse(int epoll_fd);
	bool CheckPostFlag();
	bool CloseClientFd();
	bool CheckCRequestFlag(std::string& buffer, std::string::iterator& it, std::string& object);
	void ValidLocation(LocationConfig& location, LocationConfig& location_tmp, bool& location_flag);
	bool CheckAndChangeLocationUri(std::vector<LocationConfig>& location, const std::string& uri);
	void CombineUriAndLocationRoot(LocationConfig& location);
	void ChangeDefaultPath(const std::string& uri);
	void ChangeConfUri(const std::string& uri);
	bool CheckAndChangeRootUri(const std::string& uri);
	bool CheckFileAndCombainLocation(LocationConfig& location, std::string& object);
	bool clposeAndDeleteSocket(std::map<int, ASocket*>& socket);
};
bool CompareLocationAndUri(const std::string& new_location, const std::string& before_location);

bool SkipSpaceAndCheckEnd(const std::string& request, std::string::const_iterator& it);
bool GetSubstringUntilSpace(const std::string& request, std::string::const_iterator& it, std::string& object);
bool GetSubstringUntilCarriageReturn(const std::string& request, std::string::const_iterator& it, std::string& object);
bool isCarriagereturn(const std::string& request);
bool substring_object_until_carrige_return(std::string& buffer, std::string::iterator& it,std::string& object);

#endif