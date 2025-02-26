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
#include "Request.hpp"
#include <sys/stat.h>
#include <fcntl.h>
#include <sstream>

class Response
{
	// private:
		public:
		int _fd;
		Request _request;
		std::string _response;
		std::string _response_header;
		std::string _version;
		unsigned int _status_code;
		std::string _directory;
		std::string _filename;
		std::string _content_length;
		std::string _response_message;
		std::map<std::string, std::string> _header;
		std::string _body;
		bool eroor_flag;
		std::string _root_path;


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
void SetDirectory(const std::string& dir);
void SetFilename(const std::string& file);
bool ReadFile(Request& req);
void HandleGet(Request& req);
void ExecuteAndGetStatusCode(Request& req);
void HandleMethod(Request& req);
void ExecuteHeaderResponse(Request& req);
void GetFileSize(void);

		// void Post(std::string::iterator it, std::string request);
		// void Delete(std::string::iterator it, std::string request);
};

namespace StatusCode
{
	enum Status
	{
		OK = 200,
		BadRequest = 400,
		NotFound = 404
	};
}

namespace StatusMessage
{

}