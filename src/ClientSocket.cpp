#include "ClientSocket.hpp"

ClientSocket::ClientSocket() : _complete_post_flag(false),  _complete_parse_flag(false), _post_body_flag(false) {};

ClientSocket::ClientSocket(ServerConfig& conf) : _conf(conf),  _complete_post_flag(false),  _complete_parse_flag(false), _post_body_flag(false), _response_flag(false) {};

ClientSocket::~ClientSocket() {};

bool ClientSocket::createSocket(void)
{
	return (false);
}

bool ClientSocket::CloseClientFd()
{
	if (close(this->_fd) == -1)
		return (false);
	return (true);
}

bool ClientSocket::CheckCRequestFlag(std::string& buffer, std::string::iterator& it, std::string& object)
{
	if (this->_post_body_flag == true)
	{
		if (it != buffer.end())
			return (false);
		if (this->_request._chunk_flag == true && this->_request._chunk_finish_flag == false)
			return (false);
		this->_complete_parse_flag = true;
		return (true);
	}
	if (this->_request.GetRequestFlag() == true && this->_request.GetHostFlag() == false)
		return (false);
	if (this->_request.GetPostFlag() == true)
	{
		this->_complete_post_flag = true;
		return (true);
	}
	if (it != buffer.end())
		return (false);
	this->_complete_parse_flag = true;
	return (true);
}

bool ClientSocket::validRequest(std::string& buffer, std::string::iterator& it, std::string& object)
{
	if (object.compare("\r\n") == 0)
	{
		if (this->_request.GetRequestFlag() == false)
			return (true);
		if (CheckCRequestFlag(buffer, it, object) == false)
			throw (RequestException(400, "request_flag"));
	}
	else
	{
		if (this->_request.ParseRequest(object, this->_complete_post_flag) == false)
			throw (RequestException(400, "location_error"));
		if (this->_complete_post_flag == true)
			this->_post_body_flag = true;
	}
	return (true);
}

void ClientSocket::ChangeDefaultPath(const std::string& uri)
{
	std::string object;

	object = "/etc/nginx/html";
	object += uri;
	if (object[object.length() - 1] != '/')
		object += '/';
	this->_response.setDirectory(object);
	this->_response.setFilename("index.html");
	object += "index.html";
	this->_response.setPath(object);
	return ;
}

// bool ClientSocket::CheckAndChangeRootUri(const std::string& uri)
// {
// 	std::string object;
// 	std::string root;
// 	std::string path;

// 	root = this->_conf.GetRoot();
// 	if (root.empty())
// 		return (false);
// 	object = root;
// 	object += uri;
// 	if (object[object.length() - 1] != '/')
// 		object += '/';
// 	this->_response.setDirectory(object);
// 	this->_response.setFilename(this->_conf.GetIndex());
// 	path = object;
// 	path += this->_conf.GetIndex();
// 	this->_response.SetPath(path);
// 	return (true);
// }

size_t MatchPathLength(const std::string& uri, LocationConfig& location)
{
	size_t i;
	size_t result;
	std::string path;

	i = 0;
	result = 0;
	path = location.path;
	if (uri.empty() || path.empty())
		return (0);
	while (uri[i] != '\0' && path[i] != '\0' && uri[i] == path[i])
		i++;
	return (i);
}

bool ClientSocket::CheckFileAndCombainLocation(LocationConfig& location, std::string& object)
{
	std::string file;

	file = this->_request.getFile();
	if (file.empty())
		return (false);
	this->_response.setDirectory(object);
	this->_response.setFilename(file);
	this->_response.setPath(object);
	return (true);
}

void ClientSocket::CombineUriAndLocationRoot(LocationConfig& location)
{
	std::string object;
	std::string path;

	object = location.GetRoot();
	object += this->_request.getDirectory();
	if (CheckFileAndCombainLocation(location, object) == true)
		return ;
	if (object[object.length() - 1] != '/')
		object += '/';
	this->_response.setDirectory(object);
	this->_response.setFilename(location.GetIndex());
	path = object;
	path += location.GetIndex();
	this->_response.setPath(path);
	return ;
}

bool ClientSocket::CheckAndChangeLocationUri(std::vector<LocationConfig>& location, const std::string& uri)
{
	LocationConfig location_tmp;
	size_t length;
	size_t result;
	std::vector<LocationConfig>::iterator it;
	bool location_flag;

	location_flag = false;
	it = location.begin();
	length = 0;
	result = 0;
	// todo  conf / uri /wtmの時、一部でも一致してたらOKにしないと
	for (; it != location.end(); it++)
	{
		result = MatchPathLength(uri, *it);
		if (result > length)
		{
			length = result;
			location_tmp = *it;
		}
		// ValidLocation(*it, location_tmp, location_flag);
	}
	if (length == 0)
		return (false);
	CombineUriAndLocationRoot(location_tmp);
	return (true);
}

void ClientSocket::ChangeConfUri(const std::string& uri)
{
	std::vector<LocationConfig> location;

	location = this->_conf.GetLocation();
	if (CheckAndChangeLocationUri(location, uri) == true)
		return ;
	// if (CheckAndChangeRootUri(uri) == true)
	// 	return ;
	ChangeDefaultPath(uri);
}


bool ClientSocket::checkExecuteResponse(int epoll_fd)
{
	std::string buffer = this->_buffer;
	std::string object;
	std::string::iterator it;
	struct epoll_event event;

	it = buffer.begin();
	try
	{
		while (it != buffer.end())
		{
			if (substring_object_until_carrige_return(buffer, it, object) == false)
			{
				this->_buffer = std::string(it, buffer.end());
				return (true);
			}
			else
			{
				if (this->validRequest(buffer, it, object) == 	false)
					return (false);
			}
			if (this->_complete_parse_flag == true)
			{
				if (it != buffer.end())
					return (false);
				ChangeConfUri(this->_request.getPath());
				struct epoll_event ev;
				ev.events = EPOLLOUT;
				ev.data.fd = this->_fd;

				if (epoll_ctl(epoll_fd, EPOLL_CTL_MOD,this->_fd, &ev) == -1) {
					perror("epoll_ctl: mod");
				}
				this->_response_flag = true;
				this->_response.setFd(this->_fd);
			// if (this->_file == "php" || this->_request._method != "GET")
			// 	ExecuteCgi(epoll_fd, this->_request, server_conf);
			return (true);
		}
	}
	}
	catch (const RequestException& e)
	{
		struct epoll_event ev;
		std::cout << e.what() << std::endl;
		ev.events = EPOLLOUT;
		ev.data.fd = this->_fd;
		if (epoll_ctl(epoll_fd, EPOLL_CTL_MOD,this->_fd, &ev) == -1) {
				perror("epoll_ctl: mod");
			}
		this->_request._status_number = e.getStatus();
		this->_response_flag = true;
		this->_response.setFd(this->_fd);
		return (true);
	}
	catch (std::exception& e)
	{
		std::cout << e.what() << std::endl;
		return (false);
	}
	if (it == buffer.end())
		this->_buffer.clear();
	return (true);
}



bool ClientSocket::handleEpollInEvent(int epoll_fd, std::map<int, ASocket*>& _socket)
{
	int byte_size = 0;
	char buf[BUFFER_SIZE];
	size_t pos = 0;
	
	byte_size = read(this->_fd, buf, BUFFER_SIZE);
	if (byte_size < 0)
		return (false);
	this->_buffer.append(buf, byte_size);
	pos = this->_buffer.find("\r\n");
	if (pos == std::string::npos)
		return (true);
	else
	{
		if (this->checkExecuteResponse(epoll_fd) == false)
			return (false);
	}
	return (true);
}

bool ClientSocket::clposeAndDeleteSocket(std::map<int, ASocket*>& socket)
{
	std::map<int, ASocket*>::iterator it;

	it = socket.find(this->_fd);
	if (it == socket.end())
		return (false);
	close(it->first);
	delete (it->second);
	socket.erase(it);
	return (true);
}

bool ClientSocket::handleEpollOutEvent(int epoll_fd, std::map<int, ASocket*>& socket)
{
	if (this->_response_flag == false)
		return (false);
	try
	{
		this->_response.ExecuteResponse(this->_request);
		struct epoll_event ev;
		ev.events = EPOLLIN;
		ev.data.fd = this->_fd;

		if (epoll_ctl(epoll_fd, EPOLL_CTL_MOD,this->_fd, &ev) == -1) {
			perror("epoll_ctl: mod");
		}
		return (true);
	}
	catch (const ResponseException& e)
	{
		struct epoll_event ev;
		ev.events = EPOLLIN;
		ev.data.fd = this->_fd;
		if (epoll_ctl(epoll_fd, EPOLL_CTL_DEL,this->_fd, &ev) == -1) {
				return (false);
			}
		if (clposeAndDeleteSocket(socket) == false)
			return (false);
		return (true);
	}
	catch (std::exception& e)
	{
				std::cout <<"22"<<std::endl;
		return (false);
	}
}
