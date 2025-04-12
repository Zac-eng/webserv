#include "ClientSocket.hpp"

ClientSocket::ClientSocket() : _first_post_flag(false),  _complete_post_flag(false),  _complete_parse_flag(false), _post_body_flag(false) {};

ClientSocket::ClientSocket(ServerConfig& conf) : _conf(conf), _first_post_flag(false),  _complete_post_flag(false),  _complete_parse_flag(false), _post_body_flag(false), _response_flag(false) {};

ClientSocket::~ClientSocket() {};

bool ClientSocket::CreateSocket(void)
{
	return (false);
}

bool ClientSocket::CloseClientFd()
{
	if (close(this->_fd) == -1)
		throw std::runtime_error("close fd");
	return (true);
}

bool ClientSocket::CheckPostFlag()
{
	if (this->_complete_post_flag == true)
		return (false);
	if (this->_first_post_flag == false)
	{
		this->_first_post_flag = true;
		return (true);
	}
	if (this->_complete_post_flag == false)
		this->_complete_post_flag = true;
	return (true);
}

bool ClientSocket::CheckCRequestFlag(std::string& buffer, std::string::iterator& it, std::string& object)
{
	if (this->_post_body_flag == true)
	{
		if (it != buffer.end())
			return (false);
		this->_complete_parse_flag = true;
		return (true);
	}
	if (this->_request.GetRequestFlag() == true && this->_request.GetHostFlag() == false)
		return (false);
	if (this->_request.GetPostFlag() == true)
	{
		if (CheckPostFlag() == false)
			return (false);
		return (true);
	}
	if (it != buffer.end())
		return (false);
	this->_complete_parse_flag = true;
	return (true);
}

bool ClientSocket::ValidRequest(std::string& buffer, std::string::iterator& it, std::string& object)
{
	if (object.compare("\r\n") == 0)
	{
		if (this->_request.GetRequestFlag() == false)
			return (true);
		if (CheckCRequestFlag(buffer, it, object) == false)
			return (false);
	}
	else
	{
		if (this->_first_post_flag == true)
			this->_complete_post_flag = true;
		if (this->_request.ParseRequest(object, this->_complete_post_flag) == false)
			return (false);
		if (this->_complete_post_flag == true)
			this->_post_body_flag = true;
	}
	return (true);
}

bool CompareLocationAndUri(const std::string& new_location, const std::string& before_location)
{
	if (new_location.length() > before_location.length())
		return (false);
	return (true);
}

bool LoopCheckPath(std::string& uri, std::string& location_uri, bool& location_flag)
{
	std::string tmp_location;
	std::string tmp_uri;
	std::string object;
	std::string::iterator it;
	int index;

	it = location_uri.begin();
	it++;
	index = 1;
	while (it != location_uri.end())
	{
		for (; it != location_uri.end() && *it != '/'; it++)
		{
			tmp_location = *it;
			tmp_uri += uri[index++];
		}
		if (tmp_location != tmp_uri)
			return (false);
		object += tmp_location;
		tmp_location.clear();
		tmp_uri.clear();
		if (it == location_uri.end())
			break ;
		index++;
		it++;
	}
	location_flag = true;
	return (true);
}

void ClientSocket::ValidLocation(LocationConfig& location, LocationConfig& location_tmp, bool& location_flag)
{
	std::string uri;
	std::string location_uri;

	uri = this->_request.GetUri();
	location_uri = location.GetPath();
	if (uri.length() < location_uri.length())
		return ;
	if (LoopCheckPath(uri, location_uri, location_flag) == false)
		return ;
	if (location_tmp.GetPath() == location.GetPath())
		return ;
	else if (CompareLocationAndUri(location_tmp.GetPath(), location_uri) == false)
		return ;
	location_tmp = location;
	location_flag = true;
	return ;
}

bool ClientSocket::CheckAndChangeLocationUri(std::vector<LocationConfig>& location, const std::string& uri)
{
	LocationConfig location_tmp;
	std::vector<LocationConfig>::iterator it;
	bool location_flag;

	location_flag = false;
	it = location.begin();
	location_tmp = *it;
	// todo  conf / uri /wtmの時、一部でも一致してたらOKにしないと
	for (; it != location.end(); it++)
	{
	std::cout << this->_request._path<< std::endl;
		if (this->_request.GetUri() == it->GetPath())
		{
			location_flag = true;
			location_tmp = *it;
			break ;
		}
		
		ValidLocation(*it, location_tmp, location_flag);
	}
	// if (location_flag == false)
	// 	return (false);
	CombineUriAndLocationRoot(location_tmp);
	return (true);
}


void ClientSocket::CombineUriAndLocationRoot(LocationConfig& location)
{
	std::string object;
	std::string path;

	object = location.GetRoot();
	object += this->_request.GetUri();
	this->_response.SetDirectory(object);
	this->_response.SetFilename(location.GetIndex());
	path = object;
	path += location.GetIndex();
	this->_response.SetPath(path);
	return ;
}

void ClientSocket::ChangeDefaultPath(const std::string& uri)
{
	std::string object;

	object = "/etc/nginx/html";
	object += uri;
	this->_response.SetDirectory(object);
	this->_response.SetFilename("index.html");
	object += "index.html";
	this->_response.SetPath(object);
	return ;
}

bool ClientSocket::CheckAndChangeRootUri(const std::string& uri)
{
	std::string object;
	std::string root;
	std::string path;

	root = this->_conf.GetRoot();
				
				std::cout <<root << std::endl;
	if (root.empty())
		return (false);
	object = root;
	object += uri;
	this->_response.SetDirectory(object);
	this->_response.SetFilename(this->_conf.GetIndex());
	path = object;
	path += this->_conf.GetIndex();
	this->_response.SetPath(path);
	return (true);
}

void ClientSocket::ChangeConfUri(const std::string& uri)
{
	std::vector<LocationConfig> location;

	location = this->_conf.GetLocation();
	if (CheckAndChangeLocationUri(location, uri) == true)
		return ;
	if (CheckAndChangeRootUri(uri) == true)
		return ;
	ChangeDefaultPath(uri);
}


bool ClientSocket::CheckExecuteResponse(int epoll_fd)
{
	std::string buffer = this->_buffer;
	std::string object;
	std::string::iterator it;
	struct epoll_event event;

	it = buffer.begin();
	while (it != buffer.end())
	{
		if (SubstringObject(buffer, it, object) == false)
		{
			this->_buffer = std::string(it, buffer.end());
			return (true);
		}
		else
		{
			if (this->ValidRequest(buffer, it, object) == false)
				return (false);

		}
		if (this->_complete_parse_flag == true)
		{
			if (it != buffer.end())
				return (false);

			ChangeConfUri(this->_request.GetUri());
			struct epoll_event ev;
			ev.events = EPOLLOUT;
			ev.data.fd = this->_fd;

			if (epoll_ctl(epoll_fd, EPOLL_CTL_MOD,this->_fd, &ev) == -1) {
				perror("epoll_ctl: mod");
			}
			this->_response_flag = true;
			this->_response.setFd(this->_fd);
			// if (this->_file == "php")
			// 	ExecuteCgi(epoll_fd, this->_request, server_conf);
			return (true);
		}
	}
	if (it == buffer.end())
		this->_buffer.clear();
	return (true);
}



bool ClientSocket::HandleEpollInEvent(int epoll_fd, std::map<int, ASocket*>& _socket)
{
	int byte_size = 0;
	char buf[BUFFER_SIZE];
	size_t pos = 0;
	
	byte_size = read(this->_fd, buf, BUFFER_SIZE);
	if (byte_size < 0)
	{
		close(this->_fd);
		throw std::runtime_error("close fd");
	}
	this->_buffer.append(buf, byte_size);
	pos = this->_buffer.find("\r\n");
	if (pos == std::string::npos)
		return (true);
	else
	{
		if (this->CheckExecuteResponse(epoll_fd) == false)
			return (false);
	}
	return (true);
}

void ClientSocket::HandleEpollOutEvent(void)
{
	if (this->_response_flag == false)
		return ;
	this->_response.ExecuteResponse(this->_request);
}
