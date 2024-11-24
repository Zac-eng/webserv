#pragma once

#include <iostream>
#include <string>
#include <sys/types.h>
#include <dirent.h>
#include <vector>
#include <utility>
#include <fstream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <poll.h>
#include <sstream>
#include <map>

class Request
{
	private:
		std::string _request;
		std::string _method;
		std::string _path;
		std::string _version;
		std::map<std::string, std::string> _header;
		bool eroor_flag;
		const unsigned int _fd;
		std::string _root_path;

	public:
		Request(const unsigned int fd, const std::string &root_path);
		~Request();
		bool Get(std::string::const_iterator it, const std::string& request);
		bool SearchPath(void);
		bool GetEroorFlag(void) const;
		bool HandleFile(const std::string& filename);
		bool HandleMethod(const std::string& request);
		bool ParseHeader(std::string::const_iterator& it, const std::string& request);
		
		// void Post(std::string::iterator it, std::string request);
		// void Delete(std::string::iterator it, std::string request);

	void ParseMethod(std::string& method,const std::string& request, std::string::const_iterator& it);
	bool ParsePath(std::string::const_iterator& it, const std::string& request);
	bool ParseVersion(std::string::const_iterator& it, const std::string& request);
	bool ParseHost(std::string::const_iterator& it, const std::string& request);
};