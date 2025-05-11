#pragma once

#include <iostream>
#include <string>
#include <sys/types.h>
#include <dirent.h>
#include <vector>
#include <utility>
#include <fstream>
#include <unistd.h>
#include <sstream>
#include <map>
#include <cstring>
#include <fstream>
#include "Request.hpp"
#include <sys/stat.h>
#include <fcntl.h>
#include <sstream>
#include "ResponseException.hpp"
#include <ctime>
#include <sstream>
#include <iomanip>

class Response
{
	// private:
		public:
		int _fd;
		std::string _response;
		size_t _status_code;
		std::string _directory;
		std::string _filename;
		std::string _path;
		std::string _content_length;
		std::string _response_message;
		std::vector<std::string> _header;
		std::string _body;
		std::string _cgi_buffer;


		Response ();
		~Response();
		bool Get(std::string::const_iterator it, const std::string& request);
		bool SearchPath(void);
		bool GetEroorFlag(void) const;
		bool HandleFile(const std::string& filename);
		bool ParseHeader(std::string::const_iterator& it, const std::string& request);
		void ExecuteResponse(Request& req);
bool ExistUri(const std::string& uri);
bool ReadUri(Request& req);
// bool ReadFile(Request& req);
void handleGet(Request& req);
void handlePost(Request& req);
void handleDelete(void);
void ExecuteAndGetStatusCode(Request& req);
void HandleMethod(Request& req);
void ExecuteHeaderResponse(Request& req);
void GetFileSize(void);
void CheckFileType(std::string& file);
void CheckConnectionHeader(std::map<std::string, std::string> header);
void CreateResponseHeader(Request& req);
void CreateResponse();
void ResponseError(void);
bool IsDynamicFileType(const std::string& file);
void createDateHeader(void);
void ErrorResponse(size_t code, const std::string& title);
void ResponseBadRequest(void);
void ResponseFileNotFound(void);
void ResponseMethodNotAloowed(void);
void ResponseInternalServerError(void);
void ResponseNotImplemented(void);
void ResponseBadGateway(void);
void reSetResponse(void);








int getFd(void) const;
void setFd(const int& fd);
Request getRequest(void) const;
void setRequest(const Request& request);
std::string getResponse(void) const;
void setResponse(const std::string& response);
std::string getResponseBody(void) const;
void setResponseBody(const std::string& response_body);
size_t getStatusCode(void) const;
void setStatusCode(const size_t& status_code);
std::string getDirectory(void) const;
void setDirectory(const std::string& directory);
std::string getFilename(void) const;
void setFilename(const std::string& filename);
std::string getPath(void) const;
void setPath(const std::string& path);
std::string getContentLength(void) const;
void setContentLength(const std::string& content_length);
std::string getResponseMessage(void) const;
void setResponseMessage(const std::string& response_message);
std::vector<std::string> getHeader(void) const;
void setHeader(const std::vector<std::string>& header);
std::string getBody(void) const;
void setBody(const std::string& body);
std::string getCgiBuffer(void) const;
void setCgiBuffer(const std::string& cgi_buffer);


		// void Post(std::string::iterator it, std::string request);
		// void Delete(std::string::iterator it, std::string request);
};
