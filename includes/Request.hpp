#pragma once

#include <iostream>
#include <string>
#include <sys/types.h>
#include <vector>
#include <utility>
#include <fstream>
#include <sstream>
#include <map>
#include "RequestException.hpp"
#include <cstdlib>
#include <cctype>

class Request
{
	// private:
	public:
		std::string _request;
		std::string _method;
		std::string _path;
		std::string _directory;
		std::string _file;
		std::string _extension;
		std::string _version;
		std::map<std::string, std::string> _header;
		std::string _body;
		bool _request_flag;
		bool _host_flag;
		bool _post_flag;
		bool _chunk_flag;
		bool _chunk_finish_flag;
		size_t _chunk_size;
		size_t _status_number;
		bool _connection_flag;
		std::vector<std::string> _valid_header_key;

		// LocationConfig _conf;

		Request();
		~Request();
		bool ParseMethod(const std::string& request, std::string::const_iterator& it);
		bool ParseUri(const std::string& request, std::string::const_iterator& it);
		bool ParseVersion(const std::string& request, std::string::const_iterator& it);
		bool parseHeader(const std::string& request);
		bool ParseRequestLine(const std::string& request);
		bool ParseRequest(const std::string& request, bool parse_post_flag);
		void insertHeaderKey(void);
bool CheckUriExtension(const std::string& uri, std::string::const_iterator& it_tmp);
bool SearchHeaderKey(std::string &key);
bool HandleHeaderKey(const std::string& request, std::string::const_iterator& it, std::string& key);
bool parseHeaderKey(const std::string& request, std::string::const_iterator& it, std::string& key);
bool HandleHeaderValue(const std::string& request, std::string::const_iterator& it, std::string& value);
bool parseHeaderValue(const std::string& request, std::string::const_iterator& it, std::string& key);
bool ValidMethod(const std::string& method);
bool ValidUri(const std::string& uri);
bool ValidVersion(const std::string& version);
bool ParseBody(const std::string& request);
bool SkipColon(const std::string& request, std::string::const_iterator& it);
bool ValidHeader(void);
bool GetRequestFlag();
bool GetHostFlag();
bool GetPostFlag();
std::string getDirectory(void);
bool parsePostBody(const std::string& request);
std::string getPath(void);
void SetUri(std::string& object);
std::string GetMethod();
std::string getFile();
void SearchChunkValue(std::string& value);
bool checkHexadecimal(char object);
bool parseChunkSize(const std::string& request);
bool parseChunkValue(const std::string& request);
bool executeChunk(const std::string& request);
bool parseChunk(const std::string& request);
void searchConnectionClose(std::string& value);
};
bool SkipSpaceAndCheckEnd(const std::string& request, std::string::const_iterator& it);
bool GetSubstringUntilSpace(const std::string& request, std::string::const_iterator& it, std::string& object);
bool GetSubstringUntilCarriageReturn(const std::string& request, std::string::const_iterator& it, std::string& object);
		bool isCarriagereturn(const std::string& request);