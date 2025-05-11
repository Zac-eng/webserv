#include "ClientSocket.hpp"
#include "Request.hpp"

ClientSocket::ClientSocket() : _response_flag(false), _complete_post_flag(false),  _complete_parse_flag(false), _post_body_flag(false) {};

ClientSocket::ClientSocket(ServerConfig& conf) : _conf(conf), _response_flag(false), _complete_post_flag(false),  _complete_parse_flag(false), _post_body_flag(false) {};

ClientSocket::~ClientSocket() {};

Request ClientSocket::getRequest(void) const
{
	return (this->_request);
}

void ClientSocket::setRequest(Request& request)
{
	this->_request = request;
	return ;
}

Response ClientSocket::getResponse(void) const
{
	return (this->_response);
}

void ClientSocket::setResponse(Response& response)
{
	this->_response = response;
	return ;
}

ServerConfig ClientSocket::getConf(void) const
{
	return (this->_conf);
}

void ClientSocket::setConf(ServerConfig& conf)
{
	this->_conf = conf;
	return ;
}

bool ClientSocket::getCarrigeReturnFlag(void) const
{
	return (this->_carrige_return_flag);
}

void ClientSocket::setCarrigeReturnFlag(bool& carrige_return_flag)
{
	this->_carrige_return_flag = carrige_return_flag;
	return ;
}


bool ClientSocket::getResponseFlag(void) const
{
	return (this->_response_flag);
}

void ClientSocket::setResponseFlag(bool& response_flag)
{
	this->_response_flag = response_flag;
	return ;
}

bool ClientSocket::getCompletePostFlag(void) const
{
	return (this->_complete_post_flag);
}

void ClientSocket::setCompletePostFlag(bool& complete_post_flag)
{
	this->_complete_post_flag = complete_post_flag;
	return ;
}

bool ClientSocket::getPostBodyFlag(void) const
{
	return (this->_post_body_flag);
}

void ClientSocket::setPostBodyFlag(bool& post_body_flag)
{
	this->_post_body_flag = post_body_flag;
	return ;
}

std::string ClientSocket::getBuffer(void) const
{
	return (this->_buffer);
}

void ClientSocket::setBuffer(std::string& buffer)
{
	this->_buffer = buffer;
	return ;
}


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

bool ClientSocket::CheckRequestFlag(std::string& buffer, std::string::iterator& it)
{
	if (this->_post_body_flag == true)
	{
		if (it != buffer.end())
			return (false);
		if (this->_request.getChunkFlag() == true && this->_request.getChunkFinishFlag() == false)
			return (false);
		this->_complete_parse_flag = true;
		return (true);
	}
	if (this->_request.getRequestFlag() == true && this->_request.getHostFlag() == false)
		return (false);
	if (this->_request.getPostFlag() == true)
	{
		this->_complete_post_flag = true;
		if (this->_request.checkPostContentLength() == true)
		{
			this->_complete_parse_flag = true;
			return (true);
		}
		// return (true);
	}
	if (it != buffer.end())
		return (false);
	this->_complete_parse_flag = true;
	return (true);
}

void ClientSocket::validRequest(std::string& buffer, std::string::iterator& it, std::string& object)
{
	if (object.compare("\r\n") == 0)
	{
		if (this->_request.getBadRequestFlag() == true || this->_request.checkBodyHeader() == true)
			throw RequestException(400, "bad request");
		if (this->_request.getRequestFlag() == false)
			return ;
		if (CheckRequestFlag(buffer, it) == false)
			throw (RequestException(400, "request_flag"));
	}
	else
	{
		if (this->_request.ParseRequest(object, this->_complete_post_flag) == false)
			throw (RequestException(400, "location_error"));
		if ((this->_request.getBody()).length() == this->_request.getBodySize())
			this->_complete_parse_flag = true;
		if (this->_complete_post_flag == true)
			this->_post_body_flag = true;
	}
	return ;
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
	std::string path;

	i = 0;
	path = location.getPath();
	if (uri.empty() || path.empty())
		return (0);
	while (uri[i] != '\0' && path[i] != '\0' && uri[i] == path[i])
		i++;
	return (i);
}

bool ClientSocket::CheckFileAndCombainLocation(std::string& object)
{
	std::string file;
	std::string path;

	file = this->_request.getFile();
	if (file.empty())
		return (false);
	this->_response.setDirectory(object);
	this->_response.setFilename(file);
	path = object;
	path += file;
	this->_response.setPath(path);
	return (true);
}


bool ClientSocket::existUri(const std::string& directory, const std::string& file)
{
	DIR *dir;
	dirent *entry;

	dir = opendir(directory.c_str());
	if (dir == NULL)
		return (false);
	entry = readdir(dir);
	while (entry != NULL)
	{
		if (strcmp(entry->d_name, file.c_str()) == 0)
		{
			closedir(dir);
			return (true);
		}
		entry = readdir(dir);
	}
	closedir(dir);
	return (false);
}


void ClientSocket::CombineUriAndLocationRoot(LocationConfig& location)
{
	std::string object;
	std::string path;
	std::string file;
	std::vector<std::string>::const_iterator it;

	object = location.getRoot();
	object += this->_request.getDirectory();
	if (object[object.length() - 1] != '/')
		object += '/';
	this->_response.setDirectory(object);
	path = object;
	if (!(this->_request.getFile()).empty())
	{
		if (this->existUri(this->_response.getDirectory(), this->_request.getFile()) == false)
		{
			throw RequestException(404,"uri fileaa not");
		}
		this->_response.setFilename(this->_request.getFile());
		path += this->_response.getFilename();
		this->_response.setPath(path);
		return ;
	}
	else
	{
		if ((location.getIndexFiles()).empty())
			return ;
		it = location.getIndexFiles().begin();
		for (; it != (location.getIndexFiles()).end(); it++)
		{
			if (this->existUri(this->_response.getDirectory(), *it) == true)
			{
				path += *it;
				this->_response.setFilename(*it);
				this->_response.setPath(path);
				return ;
			}
			path = object;
		}
		if (it == (location.getIndexFiles()).end())
			throw RequestException(404,"uri file not");
	}

	return ;
}

// bool ClientSocket::checkAllowMethod(const std::vector<std::string>& allow_method)
// {
// 	std::vector<std::string>::iterator it;

// 	if (allow_method.empty())
// 		return (true);
// 	it = allow_method.begin();
// 	for (; it != allow_method.end(); it++)
// 	{
// 		if (*it == this->_request.getMethod())
// 			return (true);
// 	}
// 	return (false);
// }


bool ClientSocket::CheckAndChangeLocationUri(std::vector<LocationConfig>& location, const std::string& uri)
{
	LocationConfig location_tmp;
	size_t length;
	size_t result;
	std::vector<LocationConfig>::iterator it;

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
	// if (checkAllowMethod(location_tmp.getMethod()) == false)
	// 	throw (RequestException(405, "Allow method"));
	CombineUriAndLocationRoot(location_tmp);
	return (true);
}

void ClientSocket::ChangeConfUri(const std::string& uri)
{
	std::vector<LocationConfig> location;


	
	location = this->_conf.getLocations();
	// デフォルトのrootパスを探し、404を探し、404のデフォルト書き込み
	// if (!location.empty())
	// {
	if (CheckAndChangeLocationUri(location, uri) == true)
		return ;
	// }
	// 	// if (serverRootIndex() == true)
	// 	// 	CombainRootAndIndex()
	// 	if (this->_conf.error_pages.count(404) > 0)
	// 	{
	// 		if (existUri(this->_conf.at(404)) == true)
	// 			throw (RequestException(404, "404 error"));
	// if (CheckAndChangeRootUri(uri) == true)
	// 	return ;
	throw (RequestException(404, "request aaaaa"));
	ChangeDefaultPath(uri);
}

bool ClientSocket::checkCarrigeReturnAndParseRequest(std::string& buffer, std::string::iterator& it)
{
	std::string object;

	if (substring_object_until_carrige_return(buffer, it, object) == false)
	{
		this->_buffer = std::string(it, buffer.end());
		return (false);
	}
	else
		this->validRequest(buffer, it, object);
	return (true);
}

bool ClientSocket::checkExistErrorPages(const std::string& path, std::string directory, std::string file)
{

	std::string::const_iterator it;
	std::string::const_iterator it_tmp;
	bool index_flag = false;
	
	it = path.begin();
	if (isSlash(path, it) == false)
		return (false);
	for (; it != path.end(); it++)
	{
		if (*it == '/')
			it_tmp = it;
		if (*it == '.')
			index_flag = true;
	}
	if (index_flag == false)
		return (false);
	if (*it_tmp == '/')
		it_tmp++;
	directory = path.substr(0, it_tmp - path.begin());
	file = path.substr(it_tmp - path.begin());
	if (this->existUri(directory, file) == false)
		return (false);
	this->_other_fd = open(path.c_str(), O_RDONLY);
	if (this->_other_fd == -1)
		throw RequestException(500, "file open error2");
	return (true);
}

bool ClientSocket::setFileSocket(std::map<int, ASocket*>& _socket, const std::string directory, const std::string file)
{
	ASocket *file_socket = new FileSocket(this->_fd, this->_request, this->_response, directory, file);
	
	file_socket->setFd(this->_other_fd);
	_socket.insert(std::make_pair(this->_other_fd, file_socket));
	if (set_nonblocking(this->_other_fd) == false)
	{
		delete file_socket;
		return (false);
	}
	return (true);
}

bool ClientSocket::checkErrorPages(int epoll_fd, size_t status, std::map<int, ASocket*>& _socket)
{
	std::map<int, std::string>::const_iterator it;
	std::string directory;
	std::string file;
	struct epoll_event event;

	event.events = EPOLLIN;
	event.data.fd = this->_other_fd;
	it = (this->_conf.getErrorPages()).find(status);
	if (it == (this->_conf.getErrorPages()).end())
		return (false);
	if (this->checkExistErrorPages(it->second, directory, file) == false)
		return (false);
	if (this->setFileSocket(_socket, directory, file) == false)
	{
		close (this->_other_fd);
		return (false);
	}
	if (epoll_ctl(epoll_fd, EPOLL_CTL_MOD,this->_other_fd, &event) == -1) {
		perror("epoll_ctl: mod");
	}
	this->_response_flag = true;
	return (true);
}

bool ClientSocket::checkReadFile(int epoll_fd, std::map<int, ASocket*>& _socket)
{
	struct epoll_event event;

	event.events = EPOLLIN;
	this->_other_fd = open((this->_response.getPath()).c_str(), O_RDONLY);
	if (this->_other_fd == -1)
	{
		throw RequestException(500, "file open error");
	}

	if (this->setFileSocket(_socket, this->_response.getDirectory(), this->_response.getFilename()) == false)
	{
		close (this->_other_fd);
		return (false);
	}
	event.data.fd = this->_other_fd;
	if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD,this->_other_fd, &event) == -1) {
		perror("epoll_ctl: ssmod");
	}
	if (epoll_ctl(epoll_fd, EPOLL_CTL_DEL,this->_fd, NULL) == -1) {
		perror("epoll_ctl: ssmod");
	}
	this->_response_flag = true;
	return (true);
}


bool ClientSocket::checkExecuteResponse(int epoll_fd, std::map<int, ASocket*>& _socket)
{
	std::string buffer = this->_buffer;
	std::string::iterator it;
	struct epoll_event ev;


	it = buffer.begin();
	try
	{
		while (it != buffer.end())
		{
			if (this->checkCarrigeReturnAndParseRequest(buffer, it) == false)
				return (true);
			if (this->_complete_parse_flag == true)
			{

				if (it != buffer.end())
					return (false);
				// if (this->_request.CheckMethodAndHeader() == false)
				// 	return (false);
				ChangeConfUri(this->_request.getPath());
				if (this->checkReadFile(epoll_fd, _socket) == true)
				{
					return (true);
				}
				ev.events = EPOLLOUT;
				ev.data.fd = this->_fd;
				if (epoll_ctl(epoll_fd, EPOLL_CTL_MOD,this->_fd, &ev) == -1) {
					perror("epoll_ctl: mod");
				}
				this->_response_flag = true;
				this->_response.setFd(this->_fd);
			if (this->_request.getExtension() == "php")
			{
				if (this->_request.getPostFlag() == true)
					throw (RequestException(405, "extension"));

				// ExecuteCgi(epoll_fd, this->_request, server_conf);
			}
			return (true);
		}
	}
	}
	catch (const RequestException& e)
	{
		if (checkErrorPages(epoll_fd, e.getStatus(), _socket) == true){
			return (true);
		}
		if (epoll_ctl(epoll_fd, EPOLL_CTL_MOD,this->_fd, &ev) == -1) {
				perror("epoll_ctl: aaa");
			}

		if (this->_request.getStatusNumber() != 0)
			this->_request.setStatusNumber(e.getStatus());
		this->_response_flag = true;
		this->_response.setFd(this->_fd);
		return (true);
	}
	catch (std::exception& e)
	{

		if (epoll_ctl(epoll_fd, EPOLL_CTL_DEL,this->_fd, &ev) == -1) {
				perror("epoll_ctl: md");
			}
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
		if (this->checkExecuteResponse(epoll_fd, _socket) == false)
		{
			if (clposeAndDeleteSocket(_socket) == false)
				return (false);
		}
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
	struct epoll_event ev;
	if (this->_response_flag == false)
		return (false);
	try
	{
		std::cout << this->_response.getFd()<<std::endl;
		if (this->_request.getStatusNumber() != 0)
			throw (ResponseException(this->_request.getStatusNumber()));
		this->_response.ExecuteResponse(this->_request);
		if (this->_request.getConnectionFlag() == true)
		{
			if (clposeAndDeleteSocket(socket) == false)
				return (false);
			return (true);
		}
		ev.events = EPOLLIN;
		ev.data.fd = this->_fd;
		if (epoll_ctl(epoll_fd, EPOLL_CTL_MOD,this->_fd, &ev) == -1) {
			perror("epoll_ctl: mod");
		}
		return (true);
	}
	catch (const ResponseException& e)
	{
		ev.events = EPOLLIN;
		ev.data.fd = this->_fd;
		this->_response.ResponseError(e.getStatus());
		if (epoll_ctl(epoll_fd, EPOLL_CTL_DEL,this->_fd, &ev) == -1) {
				return (false);
			}
		if (clposeAndDeleteSocket(socket) == false)
			return (false);
		return (true);
	}
	catch (std::exception& e)
	{
		if (epoll_ctl(epoll_fd, EPOLL_CTL_DEL,this->_fd, &ev) == -1) {
				perror("epoll_ctl: mod");
			}
		if (clposeAndDeleteSocket(socket) == false)
			return (false);
		return (false);
	}
}
