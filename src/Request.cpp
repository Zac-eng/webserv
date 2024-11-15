#include "Request.hpp"

bool	Request::ParseHeader(std::string::const_iterator it, const std::string& request)
{
	std::string key;
	std::string value;
	while (it != request.end())
	{
		if (it != request.end() && *it == '\r')
		{
			it++;
			if (it != request.end() || *it == '\n')
			{
				it++;
				break;
			}
		}
		while (std::isspace(*it))
			it++;
		while (it != request.end() && *it != ':' && !std::isspace(*it))
		{
			key += *it;
			it++;
		}
		if (it == request.end() || *it != ':')
		{
			std::cerr << "Invalid header" << std::endl;
			return (false);
		}
		it++;
		while (std::isspace(*it))
			it++;
		while (it != request.end() && *it != '\n' && !std::isspace(*it))
		{
			value += *it;
			it++;
		}
		if (std::isspace(*it))
		{
			it++;
		}
		this->_headers.push_back(std::make_pair(key, value));
		if (it == request.end() || *it == '\r')
		{
			it++;
		}
		if (it == request.end() || *it == '\n')
		{
			it++;
		}
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


bool	Request::Get(std::string::const_iterator it, const std::string& request)
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
	return (true);
	while (it != request.end() && std::isspace(*it))
		it++;
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
		this->_host += *it;
		it++;
	}
	if (this->_host != "Host:")
	{
		std::cerr << "Invalid host" << std::endl;
		return (false);
	}
	while (it != request.end() && *it != '\n' && std::isspace(*it))
	{
		this->_host += *it;
		it++;
	}
	std::cout << "Method: " << this->_method << std::endl;
	std::cout << "Path: " << this->_path << std::endl;
	std::cout << "Version: " << this->_version << std::endl;
	std::cout << "Host: " << this->_host << std::endl;
	return (true);
	ParseHeader(it, request);
}

bool Request::HandleMethod(const std::string& request)
{
	this->_request = request;
	std::string::const_iterator it = request.begin();

	while (it != request.end() && std::isspace(*it))
		it++;
	std::string method;
	while (it != request.end() && !std::isspace(*it))
	{
		method += *it;
		it++;
	}
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