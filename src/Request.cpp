#include "Request.hpp"

Request::Request()
{
}

void	Request::serchPath(void)
{
	std::string filename = this->_path;
	size_t pos = filename.find_last_of("/");
	if (pos != std::string::npos)
	{
		filename = filename.substr(pos + 1);
	}
	DIR *dir = opendir(filename.c_str());
	if (dir == NULL)
	{
		std::cerr << "opendir failed" << std::endl;
		return ;
	}
	dirent *entry = readdir(dir);
	if (entry == NULL)
	{
		std::cerr << "readdir failed" << std::endl;
		return ;
	}
	while (entry != NULL)
	{
		std::cout << "Name: " << entry->d_name << ", ";
		if (entry->d_type == DT_REG && filename == entry->d_name)
	}
}

void	Request::Get(std::string::const_iterator it, const std::string& request)
{
	while (it != request.end() && std::isspace(*it))
		it++;
	if (*it != '/')
	{
		std::cerr << "Invalid path" << std::endl;
		return ;
	}
	while (it != request.end() && !std::isspace(*it))
	{
		this->_path += *it;
		it++;
	}
	serchPath();
	while (it != request.end() && std::isspace(*it))
		it++;
	if (*it != 'H')
	{
		std::cerr << "Invalid version" << std::endl;
		return ;
	}
	while (it != request.end() && *it != '\n' && !std::isspace(*it))
	{
		this->_version += *it;
		it++;
	}
	if (this->_version != "HTTP/1.1")
	{
		std::cerr << "Invalid version" << std::endl;
		return ;
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
		return ;
	}
	while (it != request.end() && *it != '\n' && !std::isspace(*it))
	{
		this->_host += *it;
		it++;
	}
	if (this->_host != "Host:")
	{
		std::cerr << "Invalid host" << std::endl;
		return ;
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
}

Request::Request(const std::string& request)
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
		this->Get(it, request);
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
		return ;
	}
}

Request::~Request()
{
}