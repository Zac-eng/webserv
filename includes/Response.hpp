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

class Response
{
	// private:
		public:
		int _fd;
		Request _request;
		std::string _response;
		std::string _response_body;
		std::string _version;
		size_t _status_code;
		std::string _directory;
		std::string _filename;
		std::string _path;
		std::string _content_length;
		std::string _response_message;
		std::vector<std::string> _header;
		std::string _body;
		bool eroor_flag;
		std::string _root_path;
		std::string _cgi_buffer;


		Response(Request& request);
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
void setDirectory(const std::string& dir);
void setFilename(const std::string& file);
bool ReadFile(Request& req);
void handleGet(Request& req);
void handlePost(Request& req);
void handleDelete(Request& req);
void ExecuteAndGetStatusCode(Request& req);
void HandleMethod(Request& req);
void ExecuteHeaderResponse(Request& req);
void GetFileSize(void);
void setPath(const std::string& path);
void CheckFileType(std::string& file);
void CheckConnectionHeader(std::map<std::string, std::string> header);
void CreateResponseHeader(Request& req);
void CreateResponse();
void setFd(int fd);
void ResponseError(Request& req);
bool IsDynamicFileType(const std::string& file);
		// void Post(std::string::iterator it, std::string request);
		// void Delete(std::string::iterator it, std::string request);
};
