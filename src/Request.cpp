#include "Request.hpp"

Request::Request() : _request_flag(false), _host_flag(false), _post_flag(false),  _chunk_flag(false), _chunk_finish_flag(false), _chunk_size(0), _status_number(0), _connection_flag(false), _bad_request_flag(false), _multipart_flag(false), _progress_multipart_flag(false), _start_flag(false), _end_flag(false), _count_body(0), _max_body_size(-1)
{
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

const std::string& Request::getFile(void) const
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

std::string Request::getPathInfo(void) const
{
	return (this->_path_info);
}

void Request::setPathInfo(const std::string& body)
{
	this->_path_info = body;
	return ;
}


const std::map<std::string, std::string>& Request::getHeader(void) const
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

size_t Request::getBodySize(void) const
{
	return (this->_body_size);
}

void Request::setBodySize(const size_t& body_size)
{
	this->_body_size = body_size;
	return ;
}

bool Request::getConnectionFlag(void) const
{
	return (this->_connection_flag);
}

void Request::setQuery(const std::string& query)
{
	this->_query = query;
}
std::string Request::getQuery(void) const
{
	return (this->_query);
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

bool Request::getBadRequestFlag(void) const
{
	return (this->_bad_request_flag);
}

void Request::setBadRequestFlag(const bool& bad_request_flag)
{
	this->_bad_request_flag = bad_request_flag;
	return ;
}

bool Request::getMultipartFlag()
{
	return (this->_multipart_flag);
}

bool Request::getProgressMultipartFlag()
{
	return (this->_progress_multipart_flag);
}

void Request::setMaxBodySize(const long& max_body_size)
{
	this->_max_body_size = max_body_size;
}

void Request::setBoundary(const std::string& boundary)
{
	this->_boundary = boundary;
}


// bool Request::SearchHeaderKey(std::string &key)
// {
// 	for (size_t i = 0; i < _valid_header_key.size(); i++)
// 	{
// 		if (key == _valid_header_key[i])
// 			return (true);
// 	}
// 	return (false);
// }

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
	// if (SearchHeaderKey(key) == false)
	// 	return (false);
	return (true);
}

bool Request::parseHeaderKey(const std::string& request, std::string::const_iterator& it, std::string& key)
{
	if (SkipSpaceAndCheckEnd(request, it) == false)
		return (false);
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

void Request::checkContentLengthValue(const std::string& value)
{
	std::string::const_iterator it;
	std::string body;

	it = value.begin();
	for (; it != value.end() && *it != '\r'; it++)
	{
		if (!std::isdigit(*it))
		{
			this->_bad_request_flag = true;
				return ;
		}
		body += *it;
	}
	this->_body_size = string_to_int(body);
	return ;
}

void Request::checkMultipartHeader(std::string& value)
{
	std::string boundary;
	size_t pos;

	boundary = "boundary=";
	pos = value.find(boundary);
	if (pos == std::string::npos)
		return ;
	boundary = value.substr(pos + boundary.length());
	pos = boundary.find_first_of(" ;");
	this->_boundary = "--";
	if (pos == std::string::npos)
		this->_boundary += boundary.substr(0, pos);
	else
		this->_boundary += boundary;
	this->_multipart_flag = true;
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
		std::cout << request <<std::endl;
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
	if (key == "transfer-encoding")
		SearchChunkValue(value);
	if (key == "content-length")
		checkContentLengthValue(value);
	if (key == "connection")
		searchConnectionClose(value);
	if (key == "content-type" && this->_post_flag == true)
		checkMultipartHeader(value);
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

// void Request::CheckUriExtensionAndQuery(const std::string& uri, std::string::const_iterator& it_tmp)
// {
// 	std::string::const_iterator it;
// 	std::string extension;

// 	it_tmp++;
// 	if (it_tmp == uri.end())
// 	{
// 		this->_file = uri.substr(it_tmp - uri.begin());
// 		return ;
// 	}
// 	it = it_tmp;
// 	for (; it != uri.end(); it++)
// 	{
// 		if (*it == '?')
// 		{
// 			break ;
// 		}
// 		extension += *it;
// 	}
// 	this->_extension = extension;
// 	std::cout << "aa"<<std::endl;
// 	if (it == uri.end())
// 		this->_file = uri.substr(it_tmp - uri.begin());
// 	else
// 	{
// 		this->_file = uri.substr(it_tmp - uri.begin(), it - it_tmp);
// 		std::cout <<"aa"<< *it << std::endl;
// 		it++;
// 		std::cout << *it << std::endl;
// 		this->_query = uri.substr(it - uri.begin());
// 	}
// 	return ;
// }

// bool Request::ValidUri(const std::string& uri)
// {
// 	std::string::const_iterator it;
// 	std::string::const_iterator it_tmp;
// 	bool index_flag = false;
// 	it = uri.begin();

// 	if (isSlash(uri, it) == false)
// 		return (false);
// 	for (; it != uri.end(); it++)
// 	{
// 		if (*it == '/')
// 			it_tmp = it;
// 		if (*it == '.')
// 			index_flag = true;
// 	}
// 	if (index_flag == false)
// 	{
// 		this->_directory = uri;
// 		this->_path = uri;
// 		return (true);
// 	}
// 	if (*it_tmp == '/')
// 		it_tmp++;
// 	this->_directory = uri.substr(0, it_tmp - uri.begin());
// 	if (it_tmp == uri.end())
// 	{
// 		this->_path = this->_directory;
// 		return (true);
// 	}
// 	it = it_tmp;
// 	if (index_flag == true)
// 		this->_file = uri.substr(it_tmp - uri.begin());
// 	for (; it != uri.end(); it++)
// 	{
// 		if (*it == '.')
// 			it_tmp = it;
// 	}
// 	if (CheckUriExtension(uri, it_tmp) == false)
// 		return (false);
// 	this->_extension = uri.substr(it_tmp - uri.begin());
// 	this->_path = uri;
// 	return (true);
// }


bool Request::parseOtherUri(const std::string& uri)
{
	std::string::const_iterator it;
	std::string::const_iterator file_tmp;
	std::string::const_iterator dir_tmp;
	it = uri.begin();

	
	if (isSlash(uri, it) == false)
		return (false);
	dir_tmp = it;
	for (; it != uri.end(); it++)
	{
		if (*it == '/')
		{
			dir_tmp = it;
		}
	}
	if (dir_tmp == uri.begin())
	{
		this->_directory = *dir_tmp;
	}
	else
		this->_directory = uri.substr(0, dir_tmp - uri.begin());
	dir_tmp++;
	it = dir_tmp;
	if (dir_tmp == uri.end())
	{
		this->_path = this->_directory;
		return (true);
	}
	for (; it != uri.end(); it++)
	{
		if (*it == '.')
		{
			file_tmp = it;
			break ;
		}
	}
	if (it == uri.end())
		this->_file = uri.substr(dir_tmp - uri.begin());
	for (; it != uri.end(); it++)
	{
		if (*it == '?')
		{
			break ;
		}
	}
	if (it == uri.end())
	{
		this->_file = uri.substr(dir_tmp - uri.begin());
		file_tmp++;
		this->_extension = uri.substr(file_tmp - uri.begin());
	}
	else
	{
		this->_file = uri.substr(dir_tmp - uri.begin(), it - dir_tmp);
		file_tmp++;
		this->_extension = uri.substr(file_tmp - uri.begin() , it - file_tmp);
		if (this->_extension == "php")
		{
			it++;
			this->_query = uri.substr(it - uri.begin());
		}
		else
			this->_file = uri.substr(dir_tmp - uri.begin());
	}
	this->_path = uri;
	return (true);
}

bool Request::parseUriPathInfoPhp(const std::string& uri)
{
	std::string::const_iterator it;
	std::string::const_iterator dir_it;
	size_t pos;
	std::string tmp;
	it = uri.begin();

	it++;
	for (; it != uri.end(); it++)
	{
		if (*it == '/')
		{
			pos = tmp.find(".php");
			if (pos != std::string::npos)
				break ;
			dir_it = it;
			tmp.clear();
			continue ;
		}
		tmp += *it;
	}
	this->_directory = uri.substr(0, dir_it - uri.begin());
	dir_it++;
	it = dir_it;
	tmp.clear();
	for (; it != uri.end(); it++)
	{
		if (*it == '/')
		{
			dir_it = it;
			break ;
		}
		tmp += *it;
	}
	it--;
	this->_file = tmp;
	it+=2;
	tmp.clear();
	for (; it != uri.end(); it++)
	{
		if (*it == '?')
		{
			dir_it = it;
			break ;
		}
		tmp += *it;
	}
	if (it == uri.end())
	{
		this->_path_info = tmp;
	}
	else
	{
		it--;
		this->_path_info = tmp;
		it += 2;
	}
	this->_query = uri.substr(it - uri.begin());
	this->_path = uri;
	return (true);
}

bool Request::validUri(const std::string& uri)
{
	size_t pos;
	std::string::const_iterator it;

	pos = uri.find(".php");
	if (pos != std::string::npos)
	{
		it = uri.begin() + pos;
		it += 4;
		if (it != uri.end() && *it == '/')
		{
			this->_extension = "php";
			return (parseUriPathInfoPhp(uri));
		}
	}
		return (parseOtherUri(uri));
	return (true);
}

bool Request::ParseUri(const std::string& request, std::string::const_iterator& it)
{
	std::string uri;

	if (SkipSpaceAndCheckEnd(request, it) == false)
		return (false);
	if (GetSubstringUntilSpace(request, it, uri) == false)
		return (false);
	if (validUri(uri) == false)
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
	std::string object;

	object = version;
	if (ParseUtils::parse_object(object, "HTTP/") == false)
		return (false);
	std::cout << object << std::endl;
	if (ParseUtils::check_valid_version(object, "1.1") == false)
		return (false);
	if (!object.empty())
		return (false);
	return (true);
}

bool Request::checkBodyHeader(void)
{
	if (this->_header.count("content-length") > 0 \
		&& this->_header.count("transfer-encoding") > 0)
		return (true);
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

bool Request::checkPostContentLength(void)
{
	std::map<std::string, std::string>::iterator it;

	it = this->_header.find("content-length");
	if (it != this->_header.end())
	{
		if (it->second == "0")
			return (true);
	}
	return (false);
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
	{
		it++;
		if (it != request.end() && *it != '\r')
			return (false);
		it++;
		if (it != request.end() && *it != '\n')
			return (false);
		it++;
		if (it != request.end())
			return (false);
		this->_chunk_finish_flag = true;
		return (true);
	}
	for (; it != request.end() && *it != '\r'; it++)
	{
		std::cout << "iterator:" <<*it<< std::endl;
		if (checkHexadecimal(*it) == false)
		{
			return (false);
		}
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
		if (this->_chunk_finish_flag == true)
			return (true);
		this->_chunk_size = convertDecimal(request);
		return (true);
	}
	if (parseChunkValue(request) == false)
	{
		std::cout << "chunk_body parse" << std::endl;

		return (false);
	}
	return (true);
}

bool Request::parseChunk(const std::string& request)
{
	if (this->_chunk_finish_flag == true)
		throw (RequestException(400, "chunk_finish"));
	if (executeChunk(request) == false)
		throw (RequestException(400,"chunk_error"));
	return (true);
}

bool Request::parsePostBody(const std::string& request)
{
	if (this->_chunk_flag == true)
		return (parseChunk(request));
	this->_body += request;
	if (this->_max_body_size >= 0 && this->_body.length() > (size_t)this->_max_body_size)
		throw RequestException(413, "large request body");
	if (this->_body.length() > this->_body_size)
		throw RequestException(400, "body size");
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

void Request::reSetRequest(void)
{
	this->_request_flag = false;
	this->_host_flag = false;
	this->_post_flag =false;
	this->_chunk_finish_flag = false;
	this->_chunk_flag = false;
	this->_chunk_size = 0;
	this->_status_number = 0;
	this->_body_size = 0;
	this->_connection_flag = false;
	this->_bad_request_flag = false;
	this->_request.clear();
	this->_method.clear();
	this->_path.clear();
	this->_directory.clear();
	this->_file.clear();
	this->_extension.clear();
	this->_query.clear();
	this->_path_info.clear();
	this->_version.clear();
	this->_body.clear();
	this->_header.clear();
	this->_count_body = 0;
	this->_multipart_flag = false;
	this->_progress_multipart_flag = false;
	this->_boundary.clear();
	this->_start_flag = false;
	this->_end_flag = false;
	this->_max_body_size = -1;
}

std::string Request::substringCarrigereturn(const std::string request)
{
	std::string object;
	std::string::const_iterator it;

	it = request.begin();
	for (; it != request.end() && *it != '\r'; it++)
		object += *it;
	if (it == request.end())
		throw RequestException(400, "parse multi error");
	it++;
	if (it == request.end() || *it != '\n')
		throw RequestException(400, "parse multi error");
	it++;
	if (it == request.end())
		return (object);
	throw RequestException(400, "parse multi error");
}


void Request::parseMultipart(const std::string request)
{
	std::string object;
	std::string boundary;

	boundary = this->_boundary;
	boundary += "--";
	object = this->substringCarrigereturn(request);
	if (this->_start_flag == false)
	{
		if (object == this->_boundary)
		{
			this->_start_flag = true;
			this->_count_body += request.length();
			return ;
		}
		else
		{
			throw RequestException(400, "error");
		}
	}
	if (object == boundary)
	{
		this->_end_flag = true;
		this->_progress_multipart_flag = false;
		this->_count_body += request.length();
		if (this->_body_size != this->_count_body)
			throw RequestException(400, "diffrent body size");
		return ;
	}	
	this->_body += request;
	this->_count_body += request.length();
}

bool Request::ParseRequest(const std::string& request, bool parse_post_flag)
{
	if (parse_post_flag == true)
	{
		if (this->_multipart_flag == true)
		{
			this->_progress_multipart_flag = true;
			this->parseMultipart(request);
			return  (true);
		}
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

const std::string& Request::getBoundary() const {
	return this->_boundary;
}
