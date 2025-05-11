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
#include "FileSocket.hpp"
#include "Server.hpp"

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
	bool _complete_post_flag;
	bool _complete_parse_flag;
	bool _post_body_flag;
	std::string _buffer;
	int _other_fd;
	std::string _error_file_path;

	ClientSocket();
	~ClientSocket();
	ClientSocket(ServerConfig& conf);
	// 何もない
	bool createSocket();
	// Requestパース
	bool handleEpollInEvent(int epoll_fd, std::map<int, ASocket*>& _socket);
	// CgiSocket作成、レスポンス作成
	bool handleEpollOutEvent(int epoll_fd, std::map<int, ASocket*>& _socket);
	void validRequest(std::string& buffer, std::string::iterator& it, std::string& object);
	bool checkExecuteResponse(int epoll_fd, std::map<int, ASocket*>& _socket);
	bool CheckPostFlag();
	bool CloseClientFd();
	bool CheckRequestFlag(std::string& buffer, std::string::iterator& it);
	void ValidLocation(LocationConfig& location, LocationConfig& location_tmp, bool& location_flag);
	bool CheckAndChangeLocationUri(std::vector<LocationConfig>& location, const std::string& uri);
	void CombineUriAndLocationRoot(LocationConfig& location);
	void ChangeDefaultPath(const std::string& uri);
	void ChangeConfUri(const std::string& uri);
	bool CheckAndChangeRootUri(const std::string& uri);
	bool CheckFileAndCombainLocation(std::string& object);
	bool clposeAndDeleteSocket(std::map<int, ASocket*>& socket);
bool existUri(const std::string& directory, const std::string& file);
bool checkAllowMethod(const std::vector<std::string>& allow_method);
bool checkCarrigeReturnAndParseRequest(std::string& buffer, std::string::iterator& it);
bool checkExistErrorPages(const std::string& path, std::string directory, std::string file);
bool setFileSocket(std::map<int, ASocket*>& _socket, const std::string directory, const std::string file);
bool checkErrorPages(int epoll_fd, size_t status, std::map<int, ASocket*>& _socket);
bool checkReadFile(int epoll_fd, std::map<int, ASocket*>& _socket);


Request getRequest(void) const;
void setRequest(Request& request);
Response getResponse(void) const;
void setResponse(Response& response);
ServerConfig getConf(void) const;
void setConf(ServerConfig& conf);
bool getCarrigeReturnFlag(void) const;
void setCarrigeReturnFlag(bool& carrige_return_flag);
bool getResponseFlag(void) const;
void setResponseFlag(bool& response_flag);
bool getCompletePostFlag(void) const;
void setCompletePostFlag(bool& complete_post_flag);
bool getPostBodyFlag(void) const;
void setPostBodyFlag(bool& post_body_flag);
std::string getBuffer(void) const;
void setBuffer(std::string& buffer);

};
bool CompareLocationAndUri(const std::string& new_location, const std::string& before_location);

bool SkipSpaceAndCheckEnd(const std::string& request, std::string::const_iterator& it);
bool GetSubstringUntilSpace(const std::string& request, std::string::const_iterator& it, std::string& object);
bool GetSubstringUntilCarriageReturn(const std::string& request, std::string::const_iterator& it, std::string& object);
bool isCarriagereturn(const std::string& request);
bool substring_object_until_carrige_return(std::string& buffer, std::string::iterator& it,std::string& object);

#endif