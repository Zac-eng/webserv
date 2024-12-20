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
#include "Request.hpp"

class Response
{
	// private:
		std::string _response;
		std::string _version;
		unsigned int _status_code;
		std::string _response_message;
		std::map<std::string, std::string> _header;
		std::string _body;
		bool eroor_flag;
		std::string _root_path;

	public:
		Response();
		~Response();
		bool Get(std::string::const_iterator it, const std::string& request);
		bool SearchPath(void);
		bool GetEroorFlag(void) const;
		bool HandleFile(const std::string& filename);
		bool HandleMethod(const std::string& request);
		bool ParseHeader(std::string::const_iterator& it, const std::string& request);
		void ExecuteResponse(Request& req);
bool ExistUri(std::string& uri);
bool ReadUri(Request& req);

		
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