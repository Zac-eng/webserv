#pragma once

#include <iostream>
#include <string>
#include <sys/types.h>
#include <vector>
#include <utility>
#include <fstream>
#include <sstream>
#include <map>

class Request
{
	// private:
public:
		std::string _request;
		std::string _method;
		std::string _path;
		std::string _file;
		std::string _version;
		std::map<std::string, std::string> _header;
		std::string _body;
		bool _request_flag;
		bool _host_flag;
		bool _post_flag;
		std::vector<std::string> _valid_header_key;
		// LocationConfig _conf;

		Request();
		~Request();
		bool ParseMethod(const std::string& request, std::string::const_iterator& it);
		bool ParseUri(const std::string& request, std::string::const_iterator& it);
		bool ParseVersion(const std::string& request, std::string::const_iterator& it);
		bool ParseHeader(const std::string& request);
		bool ParseRequestLine(const std::string& request);
		bool ParseRequest(const std::string& request, bool parse_post_flag);
		void InsertHeaderKey(void);
bool CheckUriExtension(const std::string& uri, std::string::const_iterator& it_tmp);
bool SearchHeaderKey(std::string &key);
bool HandleHeaderKey(const std::string& request, std::string::const_iterator& it, std::string& key);
bool ParseHeaderKey(const std::string& request, std::string::const_iterator& it, std::string& key);
bool HandleHeaderValue(const std::string& request, std::string::const_iterator& it, std::string& value);
bool ParseHeaderValue(const std::string& request, std::string::const_iterator& it, std::string& key);
bool ValidMethod(const std::string& method);
bool ValidUri(const std::string& uri);
bool ValidVersion(const std::string& version);
bool ParseBody(const std::string& request);
bool SkipColon(const std::string& request, std::string::const_iterator& it);
bool ValidHeader(void);
bool GetRequestFlag();
bool GetHostFlag();
bool GetPostFlag();
bool ParsePostBody(const std::string& request);
std::string GetUri(void) const;
void SetUri(std::string& object);
std::string GetMethod();
std::string GetFile();

};

bool SkipSpaceAndCheckEnd(const std::string& request, std::string::const_iterator& it);
bool GetSubstringUntilSpace(const std::string& request, std::string::const_iterator& it, std::string& object);
bool GetSubstringUntilCarriageReturn(const std::string& request, std::string::const_iterator& it, std::string& object);
		bool isCarriagereturn(const std::string& request);