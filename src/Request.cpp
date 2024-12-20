#include "Request.hpp"

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
	if (ParseHeaderValue(request, it, value) == false)
	{
		// Error::InvalidHeaderValue();
		return (false);
	}
	this->_header[key] = value;
	return (true);
}

bool Request::ValidMethod(const std::string& method)
{
	if (method == "GET" || method == "POST" || method == "DELETE")
	{
		this->_method = method;
		return (true);
	}
	return (false);
}

bool Request::ParseMethod(const std::string& request, std::string::const_iterator& it)
{
	std::string method;

	if (SkipSpaceAndCheckEnd(request, it) == false)
		return (false);
	if (GetSubstringUntilSpace(request, it, _version) == false)
		return (false);
	if (ValidMethod(method) == false)
		return (false);
	return (true);
}

bool isSlash(const std::string& uri, std::string::const_iterator& it)
{
	if (it != uri.end() && *it == '/')
	{
		it++;
		return (true);
	}
	// Error::InvalidUri();
	return (false);
}

bool Request::ValidUri(const std::string& uri)
{
	std::string::const_iterator it;

	it = uri.begin();
	if (isSlash(uri, it) == false)
		return (false);
	for (; it != uri.end(); it++)
	{
		if (!std::isalnum(*it))
		{
			// Error::InvalidUri();
			return (false);
		}
	}
	return (true);
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
	return (true);
}

bool Request::VaildVersion(const std::string& version)
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
	if (GetSubstringUntilSpace(request, it, version) == false)
		return (false);
	if (ValidUri(version) == false)
		return (false);
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

bool Request::ParseRequest(const std::string& request)
{
	//終了フラグをつけるか悩み(_complete_flag)
	if (_post_flag == true)
	{
		if (ParseBody(request) == false)
			return (false);
			// return (Error::InvalidBody());
		return (true);
	}
	if (isCarriagereturn(request) == true)
	{
		if (_request_flag == false || _host_flag == false)
			return (false);
			// return (Error::MissingRequestLineAndHost(), false);
		if (_method == "POST")
			_post_flag = true;
		return (true);
	}
	if (_request_flag == false)
	{
		if (ParseRequestLine(request) == false)
			return (false);
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

void Request::InsertHeaderKey(void)
{
	_valid_header_key.clear();
	_valid_header_key.push_back("Host");
	_valid_header_key.push_back("User-Agent");
	_valid_header_key.push_back("Accept");
	_valid_header_key.push_back("Content-Type");
	_valid_header_key.push_back("Content-Length");
	_valid_header_key.push_back("Transfer-Enconding");
}

Request::Request()
{
	this->InsertHeaderKey();
	std::cout << "Request object created argument" << std::endl;
}

Request::~Request()
{
	std::cout << "Request object destroyed" << std::endl;
}