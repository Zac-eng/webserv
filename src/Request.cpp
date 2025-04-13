#include "Request.hpp"

Request::Request() : _post_flag(false), _host_flag(false), _request_flag(false)
{
	this->InsertHeaderKey();
	std::cout << "Request object created argument" << std::endl;
}

Request::~Request()
{
	std::cout << "Request object destroyed" << std::endl;
}

bool Request::GetRequestFlag()
{
	return (this->_request_flag);
}

std::string Request::GetMethod()
{
	return (this->_method);
}

std::string Request::getPath()
{
	return (this->_path);
}
std::string Request::getDirectory()
{
	return (this->_directory);
}

bool Request::GetHostFlag()
{
	return (this->_host_flag);
}

void Request::SetUri(std::string& object)
{
	this->_path = object;
}



bool Request::GetPostFlag()
{
	return (this->_post_flag);
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
	if (SearchHeaderKey(key) == false)
		return (false);
	return (true);
}

bool Request::ParseHeaderKey(const std::string& request, std::string::const_iterator& it, std::string& key)
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

bool Request::ParseHeaderValue(const std::string& request, std::string::const_iterator& it, std::string& key)
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

bool Request::ParseHeader(const std::string& request)
{
	std::string key;
	std::string value;
	std::string::const_iterator it;

	it = request.begin();
	if (ParseHeaderKey(request, it, key) == false)
	{
		// Error::InvalidHeaderKey();
		return (false);
	}
	if (SkipColon(request, it) == false)
		return (false);
	if (ParseHeaderValue(request, it, value) == false)
	{
		// Error::InvalidHeaderValue();
		return (false);
	}
	if (key == "Host")
	{
		ParseHostValue(value);
		this->_host_flag = true;
	}
	this->_header[key] = value;
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
	bool index_flag;
	it = uri.begin();

	if (isSlash(uri, it) == false)
		return (false);
	
	for (; it != uri.end(); it++)
	{
		if (*it == '/' || *it == '.')
			it_tmp = it;
	}
	// std::cout <<"bbbb"<<this->_directory<<std::endl;
	// 最後が/で終わっているか
	if (*it_tmp == '/')
	{
		this->_directory = uri;
		return (true);
	}
	if (*it_tmp == '.')
	{
		if (CheckUriExtension(uri, it_tmp) == false)
			return (false);
		this->_directory = uri.substr(0, it_tmp - uri.begin());
		this->_file = uri.substr(it_tmp - uri.begin());
	}
	return (false);
}



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
	std::cout <<this->_path<<std::endl;
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
		return (false);
	if (ParseUri(request, it) == false)
		return (false);
	if (ParseVersion(request, it) == false)
		return (false);
	if (it != request.end())
	{
		// Error::InvalidRequestLine();
		return (false);
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

bool Request::ParsePostBody(const std::string& request)
{
	std::cout << "Post Parse" << request <<std::endl;
	return (true);
}

bool Request::ParseRequest(const std::string& request, bool parse_post_flag)
{
	if (parse_post_flag == true)
	{
		if (ParsePostBody(request) == false)
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
		if (ParseHeader(request) == false)
			return (false);
			// return (Error::MissingRequestLineAndHost());
	}
	return (true);
}

std::string Request::getFile(void)
{
	return (this->_file);
}

void Request::InsertHeaderKey(void)
{
	_valid_header_key.clear();
	_valid_header_key.push_back("Host");
	_valid_header_key.push_back("User-Agent");
	_valid_header_key.push_back("Accept");
	_valid_header_key.push_back("Content-Type");
	_valid_header_key.push_back("Content-Length");
	_valid_header_key.push_back("Transfer-Enconding");
	_valid_header_key.push_back("Cashe-Control");
	_valid_header_key.push_back("Connection");
	_valid_header_key.push_back("Accept-Language");
	_valid_header_key.push_back("Accept-Encoding");
	_valid_header_key.push_back("Athorization");
	_valid_header_key.push_back("sec-ch-ua");
	_valid_header_key.push_back("sec-ch-ua-mobile");
	_valid_header_key.push_back("sec-ch-ua-platform");
	_valid_header_key.push_back("Sec-Fetch-Site");
	_valid_header_key.push_back("Sec-Fetch-User");
	_valid_header_key.push_back("Sec-Fetch-Mode");
	_valid_header_key.push_back("Sec-Fetch-Dest");
	_valid_header_key.push_back("Upgrade-Insecure-Requests");
}