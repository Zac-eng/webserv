#include "Request.hpp"

bool	Request::ParseHeader(std::string::const_iterator& it, const std::string& request)
{
	std::string host;
	std::string host_name;
	while (it != request.end())
	{
		if (it != request.end() && *it == '\r')
		{
			it++;
			if (it != request.end() || *it == '\n')
			{
				it++;
				if (it == request.end())
					return (true);
				return (false);
			}
		}
		while (std::isspace(*it))
			it++;
		while (it != request.end() && *it != ':' && !std::isspace(*it))
		{
			host += *it;
			it++;
		}
		if (it == request.end() || *it != ':')
		{
			std::cerr << "Invalid header" << host << "aaa"<< *it << std::endl;
			return (false);
		}
		it++;
		while (it != request.end() && std::isspace(*it) && *it != '\r')
			it++;
		while (it != request.end() && *it != '\r')
		{
			host_name += *it;
			it++;
		}
		if ((std::distance(it, request.end()) < 2) && *it != '\r' && *(it + 1) != '\n')
		{
			std::cerr << "Invalid Error: current char '" << *it << "', next char '" << *(it + 1) << "'" << std::endl;
			return (false);
		}
		it++;
		it++;
		this->_header[host] = host_name;
		host.clear();
		host_name.clear();
	}
	return (true);
}

bool	Request::SearchPath(void)
{
	if (this->_path == "/")
	{
		if (HandleFile(this->_root_path) == false)
			return (false);
		return (true);
	}
	size_t pos = this->_path.find_last_of("/");
	if (pos == std::string::npos)
		return (false);
	std::string dir_path = this->_path.substr(0, pos+1);
	std::string filename = this->_path.substr(pos + 1);
	std::cout << pos << "aa " << dir_path << "bb " << filename<< std::endl;
	DIR *dir = opendir(dir_path.c_str());
	if (dir == NULL)
	{
		std::cerr << "opendir failed" << std::endl;
		return (false);
	}
	dirent *entry = NULL;
	while ((entry = readdir(dir)) != NULL)
	{
		std::cout << "Name: " << entry->d_name << ", ";
		if (entry->d_type == DT_REG && filename == entry->d_name)
		{
			std::cout << "Type: Regular file" << std::endl;
			break ;
		}
	}
	if (HandleFile(filename) == false)
		return (false);
	return (true);
}

bool Request::HandleFile(const std::string& filename)
{
	std::ifstream file(filename);
	if (!file.is_open())
	{
		std::ifstream error_file("error.html");
		std::string content((std::istreambuf_iterator<char>(error_file)), std::istreambuf_iterator<char>());
		error_file.close();
		
		std::ostringstream stream;
		stream << "HTTP/1.1 404 Not Found\r\n";
		stream << "Content-Type: text/html\r\n";
		stream << "Content-Length: " << content.length() << "\r\n\r\n";
		stream << content;
		std::string response = stream.str();
		send(this->_fd, response.c_str(), response.length(), 0);
		return (true);
	}
	std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	file.close();
	std::ostringstream stream;
	stream << "HTTP/1.1 200 OK\r\n";
	stream << "Content-Type: text/html\r\n";
	stream << "Content-Length: " << content.length() << "\r\n\r\n";
	stream << content;
	std::string response = stream.str();
	send(this->_fd, response.c_str(), response.length(), 0);
	return (true);
}

bool Request::ParsePath(std::string::const_iterator& it, const std::string& request)
{
	while (it != request.end() && std::isspace(*it))
		it++;
	if (*it != '/')
	{
		std::cerr << "Invalid path" << std::endl;
		return (false);
	}
	while (it != request.end() && !std::isspace(*it))
	{
		this->_path += *it;
		it++;
	}
	if (this->SearchPath() == false)
		return (false);
	while (it != request.end() && std::isspace(*it))
		it++;
	return (true);
}

bool Request::ParseVersion(std::string::const_iterator& it, const std::string& request)
{
	if (*it != 'H')
	{
		std::cerr << "Invalid version" << std::endl;
		return (false);
	}
	while (it != request.end() && *it != '\n' && !std::isspace(*it))
	{
		this->_version += *it;
		it++;
	}
	if (this->_version != "HTTP/1.1")
	{
		std::cerr << "Invalid version" << std::endl;
		return (false);
	}
	while (it != request.end() && *it != '\n' )
	{
		it++;
	}
	if (it != request.end())
	{
		it++;
	}
	return (true);
}

bool Request::ParseHost(std::string::const_iterator& it, const std::string& request)
{
	std::string host;
	std::string host_name;

	while (it != request.end() && std::isspace(*it))
	{
		it++;
	}
	if (*it != 'H')
	{
		std::cerr << "Invalid host" << std::endl;
		return (false);
	}
	while (it != request.end() && *it != '\n' && !std::isspace(*it))
	{
		host += *it;
		it++;
	}
	if (host != "Host:")
	{
		std::cerr << "Invalid host" << std::endl;
		return (false);
	}
	while (it != request.end() && std::isspace(*it))
		it++;
	while (it != request.end() && *it != '\r' && !std::isspace(*it))
	{
		host_name += *it;
		it++;
	}
	if ((std::distance(it, request.end()) < 2) && *it != '\r' && *(it + 1) != '\n')
	{
		std::cerr << "Invalid Error: current char '" << *it << "', next char '" << *(it + 1) << "'" << std::endl;
		return (false);
	}
	it++;
	it++;
	this->_header["Host"] = host_name;
	return (true);
}

bool	Request::Get(std::string::const_iterator it, const std::string& request)
{
	if (ParsePath(it, request) == false)
		return (false);
	if (ParseVersion(it, request) == false)
		return (false);
	std::cout << "Method: " << this->_method << std::endl;
	std::cout << "Path: " << this->_path << std::endl;
	std::cout << "Version: " << this->_version << std::endl;
	if (ParseHost(it, request) == false)
		return (false);
	if (ParseHeader(it, request) == false)
		return (false);
    for (std::map<std::string, std::string>::const_iterator it = _header.begin(); it != _header.end(); ++it) {
        std::cout << "header----" << it->first << ": " << it->second << std::endl;
    }
	return (true);
}

void Request::ParseMethod(std::string& method, const std::string& request, std::string::const_iterator& it)
{
	while (it != request.end() && std::isspace(*it))
		it++;
	while (it != request.end() && !std::isspace(*it))
	{
		method += *it;
		it++;
	}
	return ;
}

bool Request::HandleMethod(const std::string& request)
{
	std::string method;
	this->_request = request;
	std::string::const_iterator it = request.begin();

	ParseMethod(method, request, it);
	if (method == "GET")
	{
		this->_method = "GET";
		if (this->Get(it, request) == false)
			return (false);
		return (true);
	}
	// else if (method == "POST")
	// {
	// 	this->_method = "POST";
	// 	this->Post(it, request);
	// }
	// else if (method == "DELETE")
	// {
	// 	this->_method = "DELETE";
	// 	this->Delete(it, request);
	// }
	else
	{
		std::cerr << "Invalid method" << std::endl;
		return (false);
	}
}


Request::Request(const unsigned int fd, const std::string &root_path) : _fd(fd), _root_path(root_path)
{
	std::cout << "Request object created argument" << std::endl;
}

Request::~Request()
{
	std::cout << "Request object destroyed" << std::endl;
}