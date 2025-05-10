#include "Request.hpp"

Request::Request() : _connection_flag(false), _status_number(0), _chunk_size(0), _post_flag(false), _chunk_flag(false), _chunk_finish_flag(false), _host_flag(false), _request_flag(false)
{
	this->insertHeaderKey();
	std::cout << "Request object created argument" << std::endl;
}

Request::~Request()
{
	std::cout << "Request object destroyed" << std::endl;
}

std::string Request::getMethod(void) const
{
	return (this->_method);
}

void Request::setMethod(const std::string& method)
{
	this->_method = method;
	return ;
}

std::string Request::getPath(void) const
{
	return (this->_path);
}

void Request::setPath(const std::string& path)
{
	this->_path = path;
	return ;
}

std::string Request::getDirectory(void) const
{
	return (this->_directory);
}

void Request::setDirectory(const std::string& directory)
{
	this->_directory = directory;
	return ;
}

std::string Request::getFile(void) const
{
	return (this->_file);
}

void Request::setFile(const std::string& file)
{
	this->_file = file;
	return ;
}

std::string Request::getExtension(void) const
{
	return (this->_extension);
}

void Request::setExtension(const std::string& extension)
{
	this->_extension = extension;
	return ;
}

std::string Request::getVersion(void) const
{
	return (this->_version);
}

void Request::setVersion(const std::string& version)
{
	this->_version = version;
	return ;
}

std::string Request::getBody(void) const
{
	return (this->_body);
}

void Request::setBody(const std::string& body)
{
	this->_body = body;
	return ;
}

std::map<std::string, std::string> Request::getHeader(void) const
{
	return (this->_header);
}

void Request::setHeader(const std::map<std::string, std::string>& header)
{
	this->_header = header;
	return ;
}

bool Request::getRequestFlag(void) const
{
	return (this->_request_flag);
}

void Request::setRequestFlag(const bool& request_flag)
{
	this->_request_flag = request_flag;
	return ;
}

bool Request::getHostFlag(void) const
{
	return (this->_host_flag);
}

void Request::setHostFlag(const bool& host_flag)
{
	this->_host_flag = host_flag;
	return ;
}

bool Request::getPostFlag(void) const
{
	return (this->_post_flag);
}

void Request::setPostFlag(const bool& post_flag)
{
	this->_post_flag = post_flag;
	return ;
}

bool Request::getChunkFlag(void) const
{
	return (this->_chunk_flag);
}

void Request::setChunkFlag(const bool& chunk_flag)
{
	this->_chunk_flag = chunk_flag;
	return ;
}

bool Request::getChunkFinishFlag(void) const
{
	return (this->_chunk_finish_flag);
}

void Request::setChunkFinishFlag(const bool& chunk_finish_flag)
{
	this->_chunk_finish_flag = chunk_finish_flag;
	return ;
}

size_t Request::getChunkSize(void) const
{
	return (this->_chunk_size);
}

void Request::setChunkSize(const size_t& chunk_size)
{
	this->_chunk_size = chunk_size;
	return ;
}

size_t Request::getStatusNumber(void) const
{
	return (this->_status_number);
}

void Request::setStatusNumber(const size_t& status_number)
{
	this->_status_number = status_number;
	return ;
}

bool Request::getConnectionFlag(void) const
{
	return (this->_connection_flag);
}

void Request::setConnectionFlag(const bool& connection_flag)
{
	this->_connection_flag = connection_flag;
	return ;
}

std::vector<std::string> Request::getValidHeaderKey(void) const
{
	return (this->_valid_header_key);
}

void Request::setValidHeaderKey(const std::vector<std::string>& valid_header_key)
{
	this->_valid_header_key = valid_header_key;
	return ;
}

bool Request::SearchHeaderKey(std::string &key)
{
	for (size_t i = 0; i < _valid_header_key.size(); i++)
	{
		if (key == _valid_header_key[i])
			return (true);
	}
	return (false);
}

void convertLower(std::string& key)
{
	for (size_t i = 0; i < key.size(); i++)
		key[i] = std::tolower(key[i]);
	return ;
}

bool Request::HandleHeaderKey(const std::string& request, std::string::const_iterator& it, std::string& key)
{
	for (; it != request.end() && *it != ':'; it++)
	{
		if (std::isspace(*it))
			return (false);
		key += *it;
	}
	if (it == request.end())
		return (false);
	convertLower(key);
	if (SearchHeaderKey(key) == false)
		return (false);
	return (true);
}

bool Request::parseHeaderKey(const std::string& request, std::string::const_iterator& it, std::string& key)
{
	if (SkipSpaceAndCheckEnd(request, it) == false)
		return (false);
	std::cout <<"rrr"<<std::endl;
	if (HandleHeaderKey(request, it, key) == false)
		return (false);
	return (true);
}

bool Request::HandleHeaderValue(const std::string& request, std::string::const_iterator& it, std::string& value)
{
	if (GetSubstringUntilCarriageReturn(request, it, value) == false)
		return (false);
	if (it != request.end())
		return (false);
	return (true);
}

bool Request::parseHeaderValue(const std::string& request, std::string::const_iterator& it, std::string& key)
{
	if (SkipSpaceAndCheckEnd(request, it) == false)
		return (false);
	if (HandleHeaderValue(request, it, key) == false)
		return (false);
	return (true);
}

bool Request::SkipColon(const std::string& request, std::string::const_iterator& it)
{
	if (it == request.end() || *it != ':')
		return (false);
	it++;
	if (SkipSpaceAndCheckEnd(request, it) == false)
		return (false);
	return (true);
}

void ParseHostValue(std::string& value)
{
	std::string object;
	std::string::iterator it;

	it = value.begin();
	for (; it != value.end() && *it != ':'; it++)
		object += *it;
	value = object;
}

void Request::SearchChunkValue(std::string& value)
{
	std::string::iterator it;
	std::string tmp;

	it = value.begin();
	for (; it != value.end(); it++)
	{
		if (*it == ',')
		{
			if (tmp.empty())
				continue ;
			if (tmp == "chunked")
			{
				this->_chunk_flag = true;
				return ;
			}
			else
				tmp.clear();			
		}
		tmp += *it;
	}
	if (!tmp.empty())
	{
		if (tmp == "chunked")
		{
			this->_chunk_flag = true;
			return ;
		}
	}
	return ;
}

void Request::searchConnectionClose(std::string& value)
{
	std::string::iterator it;
	std::string tmp;

	it = value.begin();
	for (; it != value.end(); it++)
	{
		if (*it == ',')
		{
			if (tmp.empty())
				continue ;
			if (tmp == "close")
			{
				this->_connection_flag = true;
				return ;
			}
			else
				tmp.clear();			
		}
		tmp += *it;
	}
	if (!tmp.empty())
	{
		if (tmp == "close")
		{
			this->_connection_flag = true;
			return ;
		}
	}
	return ;
}

bool Request::parseHeader(const std::string& request)
{
	std::string key;
	std::string value;
	std::string::const_iterator it;

	it = request.begin();
	if (parseHeaderKey(request, it, key) == false)
	{
		// Error::InvalidHeaderKey();
		std::cout <<request<<std::endl;
		throw (RequestException(400,"header_key_error"));
	}
	if (SkipColon(request, it) == false)
		throw (RequestException(400,"header_colon_error"));
	if (parseHeaderValue(request, it, value) == false)
	{
		// Error::InvalidHeaderValue();
		throw (RequestException(400,"header_error"));
	}
	if (key == "host")
	{
		ParseHostValue(value);
		this->_host_flag = true;
	}
	if (this->_header.count(key) > 0)
	{
		if (key == "content-length" || key == "transfer-enconding" || key == "host")
			throw (RequestException(400, "重複があります。"));
	}
	convertLower(key);
	this->_header[key] = value;
	if (key == "transfer-enconding")
		SearchChunkValue(value);
	if (key == "connection")
		searchConnectionClose(value);
	return (true);
}

bool Request::ValidMethod(const std::string& method)
{
	if (method == "GET" || method == "POST" || method == "DELETE")
	{
		// if (ValidAllowMethod() == false)
		// 	return (false);
		this->_method = method;
		if (method == "POST")
			this->_post_flag = true;
		return (true);
	}
	return (false);
}

bool Request::ParseMethod(const std::string& request, std::string::const_iterator& it)
{
	std::string method;

	if (SkipSpaceAndCheckEnd(request, it) == false)
		return (false);
	if (GetSubstringUntilSpace(request, it, method) == false)
		return (false);
	if (ValidMethod(method) == false)
		return (false);
	return (true);
}

bool isSlash(const std::string& uri, std::string::const_iterator& it)
{
	if (it != uri.end() && *it == '/')
		return (true);
	// Error::InvalidUri();
	return (false);
}

bool Request::CheckUriExtension(const std::string& uri, std::string::const_iterator& it_tmp)
{
	size_t result;

	it_tmp++;
	if (it_tmp == uri.end())
		return (false);
	result = uri.find('.', it_tmp - uri.begin());
	if (result == std::string::npos)
		return (true);
	return (false);
}

bool Request::ValidUri(const std::string& uri)
{
	std::string::const_iterator it;
	std::string::const_iterator it_tmp;
	bool index_flag = false;
	it = uri.begin();

	if (isSlash(uri, it) == false)
		return (false);
	for (; it != uri.end(); it++)
	{
		if (*it == '/')
			it_tmp = it;
		if (*it == '.')
			index_flag = true;
	}
	if (index_flag == false)
	{
		this->_directory = uri;
		this->_path = uri;
		return (true);
	}
	if (*it_tmp == '/')
		it_tmp++;
	this->_directory = uri.substr(0, it_tmp - uri.begin());
	if (it_tmp == uri.end())
	{
		this->_path = this->_directory;
		return (true);
	}
	it = it_tmp;
	if (index_flag == true)
		this->_file = uri.substr(it_tmp - uri.begin());
	for (; it != uri.end(); it++)
	{
		if (*it == '.')
			it_tmp = it;
	}
	if (CheckUriExtension(uri, it_tmp) == false)
		return (false);
	this->_extension = uri.substr(it_tmp - uri.begin());
	this->_path = uri;
		// 	std::cout <<this->_directory<<std::endl;
		// std::cout <<this->_file<<std::endl;
		// std::cout <<this->_extension<<std::endl;
		// std::cout <<this->_path<<std::endl;
	return (true);
}

// bool Request::ValidUri(const std::string& uri)
// {
// 	std::string::const_iterator it;
// 	std::string::const_iterator it_tmp;
// 	bool index_flag;
// 	it = uri.begin();

// 	if (isSlash(uri, it) == false)
// 		return (false);
	
// 	for (; it != uri.end(); it++)
// 	{
// 		if (*it == '/' || *it == '.')
// 			it_tmp = it;
// 	}
// 	// std::cout <<"bbbb"<<this->_directory<<std::endl;
// 	// 最後が/で終わっているか
// 	if (*it_tmp == '/')
// 	{
// 		this->_directory = uri;
// 		return (true);
// 	}
// 	this->_file = uri.substr(it_tmp - uri.begin());
// 	if (*it_tmp == '.')
// 	{
// 		if (CheckUriExtension(uri, it_tmp) == false)
// 			return (false);
// 		this->_directory = uri.substr(0, it_tmp - uri.begin());
// 		this->_extension = uri.substr(it_tmp - uri.begin());
// 		// this->_file = 
// 		std::cout <<this->_directory<<std::endl;
// 		std::cout <<this->_file<<std::endl;
// 	}
// 	return (false);
// }



bool Request::ParseUri(const std::string& request, std::string::const_iterator& it)
{
	std::string uri;

	if (SkipSpaceAndCheckEnd(request, it) == false)
		return (false);
	if (GetSubstringUntilSpace(request, it, uri) == false)
		return (false);
	if (ValidUri(uri) == false)
		return (false);
	this->_path = uri;
	// if (CheckRootPath(uri) == true)
	// 	ReplaceDirectory(uri);
	// if (CheckIndexFile(uri) == true)
	// 	return (false);
	return (true);
}

bool Request::ValidVersion(const std::string& version)
{
	if (version == "HTTP/1.1")
		return (true);
	// Error::InvalidVersion();
	return (false);
}

bool Request::ParseVersion(const std::string& request, std::string::const_iterator& it)
{
	std::string version;

	if (SkipSpaceAndCheckEnd(request, it) == false)
		return (false);
	if (GetSubstringUntilCarriageReturn(request, it, version) == false)
		return (false);
	if (ValidVersion(version) == false)
		return (false);
	this->_version = version;
	return (true);
}

bool Request::ParseRequestLine(const std::string& request)
{
	std::string::const_iterator it = request.begin();
	if (ParseMethod(request, it) == false)
		throw (RequestException(400, "method_error"));
	if (ParseUri(request, it) == false)
		throw (RequestException(400, "uri_error"));
	if (ParseVersion(request, it) == false)
		throw (RequestException(400, "version_error"));
	if (it != request.end())
	{
		// Error::InvalidRequestLine();
		throw (RequestException(400, "request_line"));
	}
	return (true);
}

bool isCarriagereturn(const std::string& request)
{
	if (request == "\r\n")
		return (true);
	return (false);
}

bool Request::ParseBody(const std::string& request)
{
	std::string::const_iterator it = request.begin();
	size_t pos = request.find("\r\n");

	if (pos == 0 || pos == std::string::npos)
		return (false);
	it += pos + 2;
	if (it != request.end())
		return (false);
	this->_body = request.substr(0, pos);
	return (true);
}

// bool Request::ValidHeader(void)
// {
// 	if (this->_method == "GET")
// 		return (ValidHeaderGET());
// 	else if (this->_method == "POST")
// 		return (ValidHeaderPOST());
// 	else if (this->_method == "DELETE")
// 		return (ValidHeaderDELETE());
// 	return (false);
// }

bool Request::checkHexadecimal(char object)
{
	if (isdigit(object) == 0 && object != 'A' && object != 'B' &&
		object != 'C' && object != 'D' && object != 'E' && object != 'F')
		return (false);
	return (true);
}

bool Request::parseChunkSize(const std::string& request)
{
	std::string::const_iterator it;

	it = request.begin();
	if (checkHexadecimal(*it) == false)
		return (false);
	if (*it == '0')
		return (false);
	for (; it != request.end() && *it != '\r'; it++)
	{
		if (checkHexadecimal(*it) == false)
			return (false);
	}
	if (it == request.end())
		return (false);
	it++;
	if (it == request.end() || *it != '\n')
		return (false);
	it++;
	if (it != request.end())
		return (false);
	return (true);
}

bool Request::parseChunkValue(const std::string& request)
{
	std::string object;
	std::string::const_iterator it;

	it = request.begin();
	if (GetSubstringUntilCarriageReturn(request, it, object) == false)
		return (false);
	if (object.length() != this->_chunk_size)
		return (false);
	object += "\r\n";
	if (request != object)
		return (false);
	this->_body += request;
	this->_chunk_size = 0;
	return (true);
}

size_t convertDecimal(const std::string& request)
{
	char * end;
	long result;
	std::string object;
	std::string::const_iterator it;
	
	it = request.begin();
	for (; it != request.end() && *it != '\r'; it++)
		object += *it;
	// requestを16進数から10進数に変換
	result = strtol(object.c_str(), &end, 16);
	//endのポインタの位置が文字列の終端ではない。
	if (*end != '\0')
		throw RequestException(400, "convert_error");
	return (result);
}

bool Request::executeChunk(const std::string& request)
{
	if (this->_chunk_size == 0)
	{
		if (parseChunkSize(request) == false)
			return (false);
		this->_chunk_size = convertDecimal(request);
		return (true);
	}
	if (parseChunkValue(request) == false)
		return (false);
	return (true);
}

bool Request::parseChunk(const std::string& request)
{
	if (this->_chunk_finish_flag == true)
		throw (RequestException(400, "chunk_finish"));
	if (request == "\r\n")
		this->_chunk_finish_flag = true;
	if (executeChunk(request) == false)
		throw (RequestException(400,"chunk_error"));
	return (true);
}

bool Request::parsePostBody(const std::string& request)
{
	if (this->_chunk_flag == true)
		return (parseChunk(request));
	this->_body += request;
	return (true);
}

bool Request::CheckMethodAndHeader(void)
{
	std::string type;
	std::string length;

	type = "content-type";
	length = "content-length";
	if (this->_post_flag == true)
	{
		if (this->_header.find(length.c_str()) != this->_header.end() &&
			this->_header.find(type.c_str()) != this->_header.end())
			return (true);
		return (false);
	}
	return (true);
}

bool Request::ParseRequest(const std::string& request, bool parse_post_flag)
{
	if (parse_post_flag == true)
	{
		if (parsePostBody(request) == false)
			return (false);
	}
	else if (_request_flag == false)
	{
		if (ParseRequestLine(request) == false)
			return (false);
		this->_request_flag = true;
			// return (Error::MissingRequestLineAndHost());
	}
	else
	{
		if (parseHeader(request) == false)
			return (false);
			// return (Error::MissingRequestLineAndHost());
	}
	return (true);
}

void Request::insertHeaderKey(void)
{
	_valid_header_key.clear();
	_valid_header_key.push_back("host");
	_valid_header_key.push_back("date");
	_valid_header_key.push_back("user-agent");
	_valid_header_key.push_back("accept");
	_valid_header_key.push_back("content-type");
	_valid_header_key.push_back("content-length");
	_valid_header_key.push_back("transfer-enconding");
	_valid_header_key.push_back("cashe-control");
	_valid_header_key.push_back("connection");
	_valid_header_key.push_back("accept-language");
	_valid_header_key.push_back("accept-encoding");
	_valid_header_key.push_back("athorization");
	_valid_header_key.push_back("sec-ch-ua");
	_valid_header_key.push_back("sec-ch-ua-mobile");
	_valid_header_key.push_back("sec-ch-ua-platform");
	_valid_header_key.push_back("sec-fetch-site");
	_valid_header_key.push_back("sec-fetch-user");
	_valid_header_key.push_back("sec-fetch-mode");
	_valid_header_key.push_back("sec-fetch-dest");
	_valid_header_key.push_back("referer");
	_valid_header_key.push_back("if-none-match");
	// _valid_header_key.push_back("Referer");
	// _valid_header_key.push_back("Referer");
	// _valid_header_key.push_back("Referer");
	// _valid_header_key.push_back("Referer");
	// _valid_header_key.push_back("Referer");

	_valid_header_key.push_back("upgrade-insecure-requests");
}