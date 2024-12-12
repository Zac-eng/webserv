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
		std::string _body;
		bool _request_flag;
		bool _host_flag;
		bool eroor_flag;
		const unsigned int _fd;
		std::string _root_path;

		std::vector<std::string> _valid_header_key;

	public:
		Request(const unsigned int fd, const std::string &root_path);
		~Request();
		bool ParseMethod(const std::string& request, std::string::const_iterator& it);
		bool ParseUri(const std::string& request, std::string::const_iterator& it);
		bool ParseVersion(const std::string& request, std::string::const_iterator& it)
		bool ParseHeader(const std::string& request);
		bool ParseRequestLine(const std::string& request);
		bool ParseRequest(const std::string& request);
		bool isCarriagereturn(const std::string& request);
		void InsertHeaderKey(void);

};

namespace Error
{
	void MissingRequestLineAndHost(void);
	void InvalidMethod(void);
	void InvalidUri(void);
	void InvalidVersion(void);
	void InvalidRequestLine(void);
	void InvalidHeaderKey(void);
	void InvalidHeaderValue(void);
}
