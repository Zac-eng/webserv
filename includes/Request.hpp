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
	private:
		std::string _request;
		std::string _method;
		std::string _path;
		std::string _directory;
		std::string _file;
		std::string _extension;
		std::string _version;
		std::string _body;
		std::map<std::string, std::string> _header;
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
	public:
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
bool parsePostBody(const std::string& request);
void SetUri(std::string& object);
std::string GetMethod();
void SearchChunkValue(std::string& value);
bool checkHexadecimal(char object);
bool parseChunkSize(const std::string& request);
bool parseChunkValue(const std::string& request);
bool executeChunk(const std::string& request);
bool parseChunk(const std::string& request);
void searchConnectionClose(std::string& value);
bool CheckMethodAndHeader(void);


std::string getMethod(void) const;
void setMethod(const std::string& method);
std::string getPath(void) const;
void setPath(const std::string& path);
std::string getDirectory(void) const;
void setDirectory(const std::string& directory);
std::string getFile(void) const;
void setFile(const std::string& file);
std::string getExtension(void) const;
void setExtension(const std::string& extension);
std::string getVersion(void) const;
void setVersion(const std::string& version);
std::string getBody(void) const;
void setBody(const std::string& body);
std::map<std::string, std::string> getHeader(void) const;
void setHeader(const std::map<std::string, std::string>& header);
bool getRequestFlag(void) const;
void setRequestFlag(const bool& request_flag);
bool getHostFlag(void) const;
void setHostFlag(const bool& host_flag);
bool getPostFlag(void) const;
void setPostFlag(const bool& post_flag);
bool getChunkFlag(void) const;
void setChunkFlag(const bool& chunk_flag);
bool getChunkFinishFlag(void) const;
void setChunkFinishFlag(const bool& chunk_finish_flag);
size_t getChunkSize(void) const;
void setChunkSize(const size_t& chunk_size);
size_t getStatusNumber(void) const;
void setStatusNumber(const size_t& status_number);
bool getConnectionFlag(void) const;
void setConnectionFlag(const bool& connection_flag);
std::vector<std::string> getValidHeaderKey(void) const;
void setValidHeaderKey(const std::vector<std::string>& valid_header_key);






};
bool SkipSpaceAndCheckEnd(const std::string& request, std::string::const_iterator& it);
bool GetSubstringUntilSpace(const std::string& request, std::string::const_iterator& it, std::string& object);
bool GetSubstringUntilCarriageReturn(const std::string& request, std::string::const_iterator& it, std::string& object);
		bool isCarriagereturn(const std::string& request);