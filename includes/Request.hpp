#pragma once

#include <iostream>
#include <string>
#include <sys/types.h>
#include <dirent.h>

class Request
{
	private:
		std::string _request;
		std::string _method;
		std::string _path;
		std::string _version;
		std::string _host;

	public:
		Request();
		Request(const std::string& request);
		~Request();
		void Get(std::string::const_iterator it, const std::string& request);
		void serchPath(void);
		// void Post(std::string::iterator it, std::string request);
		// void Delete(std::string::iterator it, std::string request);
};