#ifndef CLIENTSOCKET_HPP
#define CLIENTSOCKET_HPP


#include "location.hpp"
#include "nginx.hpp"
#include "Request.hpp"
#include "Response.hpp"
#include "ASocket.hpp"
#include "CgiSocket.hpp"
#include <iostream>
#include <string>
#include <sys/types.h>
#include <vector>
#include <utility>
#include <fstream>
#include <sstream>
#include <map>
#include <sys/epoll.h>
#include "Server.hpp"
#include <ctime>


#define BUFFER_SIZE 2048

class ASocket;

class ClientSocket : public ASocket
{
	public:
	Request _request;
	Response _response;
	ServerConfig _conf;
	struct sockaddr_in _address;
	bool _response_flag;
	bool _progress_post_flag;
	bool _complete_parse_flag;
	bool _post_body_flag;
	std::string _buffer;
	int _other_fd;
	std::string _error_file_path;
	bool _multipart_flag;
	bool _error_file_flag;



	ClientSocket();
	~ClientSocket();
	ClientSocket(ServerConfig& conf, struct sockaddr_in _address);
	// 何もない
	bool createSocket();
	// Requestパース
	void handleEpollInEvent(int epoll_fd, std::map<int, ASocket*>& _socket);
	// CgiSocket作成、レスポンス作成
	void handleEpollOutEvent(int epoll_fd, std::map<int, ASocket*>& _socket);
	void validRequest(std::string& buffer, std::string::iterator& it, std::string& object);
	void checkExecuteResponse(int epoll_fd, std::map<int, ASocket*>& sock);
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
	void closeAndDeleteSocket(std::map<int, ASocket*>& socket);
bool existUri(const std::string& directory, const std::string& file);
bool checkAllowMethod(const std::vector<std::string>& allow_method);
bool checkCarrigeReturnAndParseRequest(std::string& buffer, std::string::iterator& it);
bool checkExistErrorPages(const std::string& path, std::string directory, std::string file);
bool setFileSocket(std::map<int, ASocket*>& _socket, const std::string directory, const std::string file);
void reSetClientSocket();
bool getFileSize(int fd);
bool readFile(int fd);
void checkErrorPages(size_t status);
void checkReadFile(void);
void parseRedirect(LocationConfig& location);
bool handleTimeOut(int epoll_fd, std::map<int, ASocket*>& _socket, int fd);
void generateAutoIndex(const std::string directory, const std::string uri);


Request getRequest(void) const;
void setRequest(Request& request);
Response getResponse(void) const;
void setResponse(Response& response);
ServerConfig getConf(void) const;
void setConf(ServerConfig& conf);
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
