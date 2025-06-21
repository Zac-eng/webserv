#include "ClientSocket.hpp"
#include "Request.hpp"

ClientSocket::ClientSocket() : _response_flag(false), _progress_post_flag(false),  _complete_parse_flag(false), _post_body_flag(false), _multipart_flag(false), _error_file_flag(false) {};

ClientSocket::ClientSocket(ServerConfig& conf, struct sockaddr_in address) : _conf(conf), _address(address), _response_flag(false), _progress_post_flag(false),  _complete_parse_flag(false), _post_body_flag(false), _multipart_flag(false), _error_file_flag(false) {};

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
	return (this->_progress_post_flag);
}

void ClientSocket::setCompletePostFlag(bool& complete_post_flag)
{
	this->_progress_post_flag = complete_post_flag;
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
	//ポストを実行中で、キャリッジリターンが送られた時の処理。chunkが終わっているか終わっていないかの確認。
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
		// this->_request.setMaxBodySize(this->_conf.getMaxBodySize());
		this->_request.setMaxBodySize(this->_conf.getClientMaxBodySize());
		this->_progress_post_flag = true;
		if (this->_request.checkPostContentLength() == true)
		{
			this->_complete_parse_flag = true;
			return (true);
		}
		return (true);
	}
	if (it != buffer.end())
		return (false);
	this->_complete_parse_flag = true;
	return (true);
}

void ClientSocket::validRequest(std::string& buffer, std::string::iterator& it, std::string& object)
{
	if (this->_multipart_flag == true)
	{
		if (this->_request.ParseRequest(object, this->_progress_post_flag) == false)
			throw (RequestException(400, "location_error"));
		if (this->_request.getProgressMultipartFlag() == false)
		{
			this->_multipart_flag = false;
			this->_complete_parse_flag = true;
		}
	}
	else if (object.compare("\r\n") == 0)
	{
		if (this->_request.getBadRequestFlag() == true || this->_request.checkBodyHeader() == true)
			throw RequestException(400, "bad request");
		if (this->_request.getRequestFlag() == false)
			return ;
		if (CheckRequestFlag(buffer, it) == false)
		{
			throw (RequestException(400, "request_flag"));
		}
	}
	else
	{
		if (this->_request.ParseRequest(object, this->_progress_post_flag) == false)
			throw (RequestException(400, "location_error"));
		if (!(this->_request.getBody()).empty() && (this->_request.getBody()).length() == this->_request.getBodySize())
			this->_complete_parse_flag = true;
		if (this->_progress_post_flag == true)
			this->_post_body_flag = true;
		if (this->_progress_post_flag == true && this->_request.getMultipartFlag() == true)
			this->_multipart_flag = true;
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
	if (this->existUri(this->_response.getDirectory(), "index.html") == true)
	{
		object += "index.html";
		this->_response.setFilename("index.html");
		this->_response.setPath(object);
		return ;
	}
	throw RequestException(404, "default error");
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
	{
		i++;
	}
		return (i);
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

void ClientSocket::generateAutoIndex(const std::string directory, const std::string uri)
{
	DIR *dir;
	dirent *entry;
	std::ostringstream body;
	std::string name;
	std::string full_path;

	dir = opendir(directory.c_str());
	if (dir == NULL)
	{
		throw RequestException(404, "open dir");
	}
	body << "<html><head><title>Index of " << uri << "</title></head></body>\n";
	body << "<h1>Index of "<< uri << "</h1><ul>\n";

	entry = readdir(dir);
	while (entry != NULL)
	{
		name = entry->d_name;
		if (name == "." || name == "..")
		{
			entry = readdir(dir);
			continue ;
		}
		std::cout <<"uri:"<<uri<<std::endl;
		if (uri.empty() || uri[uri.length() - 1] != '/')
			full_path = uri + "/" + name;
		else
			full_path = uri + name;
		body << "<li><a href=\"" << full_path << "\">" << name << "</a></li>\n";
		entry = readdir(dir);
	}
	body << "</ul></body></html>\n";
	closedir(dir);
		std::cout << _response.getCgiBuffer() << this->_fd << std::endl;

	this->_response.setBody(body.str());
	return ;
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
		if (this->_request.getExtension() == "php" && !(this->_request.getQuery()).empty())
		{
			return ;
		}
		else if (this->existUri(this->_response.getDirectory(), this->_request.getFile()) == false)
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
		{
			if (location.getautoindex() == true)
			{
				if (this->_request.getMethod() != "GET")
					throw RequestException(400, "method auto error");
				return (generateAutoIndex(object, this->_request.getDirectory()));
			}
			throw RequestException(404, "location file error");
		}
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

bool ClientSocket::checkAllowMethod(const std::vector<std::string>& allow_method)
{
	std::vector<std::string>::const_iterator it;

	if (allow_method.empty())
		return (true);
	it = allow_method.begin();
	for (; it != allow_method.end(); it++)
	{
		if (*it == this->_request.getMethod())
			return (true);
	}
	return (false);
}

void ClientSocket::parseRedirect(LocationConfig& location)
{
	std::map<int, std::string>::const_iterator redirect_map;

	// (void)location;
	// redirect_map = location.getRedirect();
	if ((location.getRedirectMap().empty()))
		throw RequestException(500, "redirect");
	redirect_map = (location.getRedirectMap()).begin();
	this->_response.setRedirectUri(redirect_map->second);
	this->_request.setStatusNumber(redirect_map->first);
	return ;
}


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
	}
	if (length == 0)
		return (false);
	if (checkAllowMethod(location_tmp.getMethod()) == false)
		throw (RequestException(405, "Allow method"));
	if (location_tmp.getRedirectFlag() == true)
	{
		std::cout << "aa"<<std::endl;
		std::cout << location_tmp.getPath()<<std::endl;
		parseRedirect(location_tmp);
	}
	else
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
	// if (CheckAndChangeRootUriredirect_(uri) == true)
	// 	return ;
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
	this->_response.setDirectory(directory);
	this->_response.setFilename(file);
	this->_response.setPath(path);
	return (true);
}


void ClientSocket::checkErrorPages(size_t status)
{
	std::map<int, std::string>::const_iterator it;
	std::string path;
	std::string directory;
	std::string file;
	std::map<int, std::string> error_pages;
	int fd;

	error_pages = this->_conf.getErrorPages();
	it = error_pages.begin();
	for (; it != error_pages.end(); it++)
	{
		if (it->first == (int)status)
		break ;
	}
	if (it == error_pages.end())
	return ;
	path = it->second;
	if (this->checkExistErrorPages(path, directory, file) == false)
		return ;
	fd = open(path.c_str(), O_RDONLY);
	if (fd == -1)
	{
		this->_request.setStatusNumber(500);
		return ;
	}
	if (readFile(fd) == false)
		this->_request.setStatusNumber(500);
	close(fd);
	this->_error_file_flag = true;
	return ;
}

bool ClientSocket::getFileSize(int fd)
{
	struct stat file;
	std::stringstream ss;
	std::string path;
	
	path = this->_response.getDirectory();
	path += this->_response.getFilename();
	if (stat(path.c_str(), &file) == -1)
	{
		close(fd);
		return (false);
	}
	ss << file.st_size;
	this->_response.setContentLength(ss.str());
	return (true);
}

bool ClientSocket::readFile(int fd)
{
	int byte_size;
	char buf[BUFFER_SIZE];
	std::string buffer;

	while (1)
	{
		byte_size = read(fd, buf, BUFFER_SIZE);
		if (byte_size < 0)
		{
			close(fd);
			return (false);
		}
		else if (byte_size == 0)
		{
			if (this->getFileSize(fd) == false)
				return (false);
			break;
		}
		else
			buffer.append(buf, byte_size);
	}
	this->_response.setBody(buffer);
	return (true);
}

void ClientSocket::checkReadFile(void)
{
	int fd;

	fd = open((this->_response.getPath()).c_str(), O_RDONLY);
	if (fd == -1)
	{
		throw RequestException(500, "file open error");
	}
	if (readFile(fd) == false)
		throw RequestException(404, "file errorrr");
	close(fd);
	// if (this->setFileSocket(_socket, this->_response.getDirectory(), this->_response.getFilename()) == false)
	// {
	// 	close (this->_other_fd);
	// 	return (false);
	// }
	// if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD,this->_other_fd, &event) == -1) {
	// 	close (this->_other_fd);
	// 	throw RequestException(500, "file open error");
	// }
	this->_response_flag = true;
	this->_start_time = -1;
	return ;
}


void ClientSocket::checkExecuteResponse(int epoll_fd, std::map<int, ASocket*>& sock)
{
	std::string buffer = this->_buffer;
	std::string::iterator it;
	struct epoll_event ev;

	ev.events = EPOLLOUT;
	ev.data.fd = this->_fd;
	it = buffer.begin();
	while (it != buffer.end())
	{
		if (this->checkCarrigeReturnAndParseRequest(buffer, it) == false)
			return ;
		if (this->_complete_parse_flag == true)
		{
			this->_start_time = -1;
			if (it != buffer.end())
				throw RequestException(400, "Parse not finish");
			if (this->_request.getMethod() == "POST" && this->_request.getExtension() == "php" && this->_request.getMultipartFlag() == false)
			{
				throw RequestException(400, "post");	
			}
			ChangeConfUri(this->_request.getPath());
			if (!(this->_response.getRedirectUri()).empty())
			{
				if (this->_request.getMethod() != "GET")
				{
					this->_request.setMethod("GET");
				}
				throw RequestException(301, "redirect");
			}
			else if (this->_request.getExtension() == "php")
			{
				this->_response_flag = true;
				this->_response.setFd(this->_fd);
				if (this->_request.getFile().empty())
					this->_request.setFile("index.php");
				if (!this->existUri(this->_response.getDirectory(), this->_request.getFile()))
					throw RequestException(404, "default error");
				this->_request.setFile(_response.getDirectory() + _request.getFile());
				CgiSocket* cgi = CgiSocket::createCgiSocket(this->_conf, this->_request, this->_address, _response._cgi_buffer, this->_fd);
				if (cgi == NULL)
					throw RequestException(this->_request.getStatusNumber(), "cgi cannot executed");
				ev.events = EPOLLOUT;
				ev.data.fd = cgi->getWritePipe();
				if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, ev.data.fd, &ev) == -1) {
					throw RequestException(500, "Parse not finish");
				}
				
				if (sock.insert(std::make_pair(cgi->getWritePipe(), cgi)).second == false)
					throw RequestException(500, "sock_map insertion failed");
				cgi->setStartTime(time(NULL));
				return ;
			}
			else
			{
				if (this->_request.getMethod() != "GET")
					throw RequestException(400, "cgi method error");
				if ((this->_response.getBody()).empty())
					this->checkReadFile();
				std::cout << this->_request.getMaxBodySize()<<"max: net"<<(this->_response.getBody()).length()<<std::endl;
			}
			if (epoll_ctl(epoll_fd, EPOLL_CTL_MOD,this->_fd, &ev) == -1) {
				throw RequestException(500, "Parse not finish");
			}
			this->_response_flag = true;
			this->_start_time = -1;
			this->_response.setFd(this->_fd);
			return ;
		}
	}
	if (it == buffer.end())
		this->_buffer.clear();
	return ;
}


void ClientSocket::handleEpollInEvent(int epoll_fd, std::map<int, ASocket*>& _socket)
{
	int byte_size = 0;
	char buf[BUFFER_SIZE];
	size_t pos = 0;
	struct epoll_event ev;

	ev.events = EPOLLOUT;
	ev.data.fd = this->_fd;
	try
	{
		byte_size = read(this->_fd, buf, BUFFER_SIZE);
		if (byte_size < 0)
			return ;
		else if (byte_size == 0)
		{
			if (epoll_ctl(epoll_fd, EPOLL_CTL_DEL,this->_fd, &ev) == -1) {
				return ;
				}
			closeAndDeleteSocket(_socket);
			return ;
		}
		if (byte_size != BUFFER_SIZE)
		{
			buf[byte_size] = '\0';
		}
		this->_buffer.append(buf, byte_size);
		pos = this->_buffer.find("\r\n");
		this->_start_time = time(NULL);
		if (pos == std::string::npos)
		{
			// if (epoll_ctl(epoll_fd, EPOLL_CTL_DEL,this->_fd, &ev) == -1) {
			// 	return ;
			// 	}
			// closeAndDeleteSocket(_socket);
			return ;
		}
		else
		{
			this->checkExecuteResponse(epoll_fd, _socket);
		}
		return ;
	}
	catch (const RequestException& e)
	{
		std::cout << e.what() << std::endl;
		if (epoll_ctl(epoll_fd, EPOLL_CTL_MOD,this->_fd, &ev) == -1) {
				this->_request.setStatusNumber(500);
			}
		this->_request.setStatusNumber(e.getStatus());
		this->_response_flag = true;
		this->_start_time = -1;;
		this->_response.setFd(this->_fd);
	}
	catch (std::exception& e)
	{
		if (epoll_ctl(epoll_fd, EPOLL_CTL_MOD,this->_fd, &ev) == -1) {
			this->_request.setStatusNumber(500);
			}
		this->_request.setStatusNumber(500);
		this->_response_flag = true;
		this->_start_time = -1;;
		this->_response.setFd(this->_fd);
	}
}

void ClientSocket::closeAndDeleteSocket(std::map<int, ASocket*>& socket)
{
	std::map<int, ASocket*>::iterator it;

	it = socket.find(this->_fd);
	if (it == socket.end())
		return ;
	close(it->first);
	delete (it->second);
	socket.erase(it);
	return ;
}

void ClientSocket::reSetClientSocket()
{
	this->_response_flag = false;
	this->_progress_post_flag = false;
	this->_complete_parse_flag = false;
	this->_post_body_flag = false;
	this->_buffer.clear();
	this->_other_fd = -1;
	this->_error_file_path.clear();
	this->_multipart_flag = false;
	this->_start_time = time(NULL);
	this->_time_out_flag = false;
}

void ClientSocket::handleEpollOutEvent(int epoll_fd, std::map<int, ASocket*>& socket)
{
	struct epoll_event ev;

	ev.events = EPOLLIN;
	ev.data.fd = this->_fd;
	try
	{
		if (this->_time_out_flag == true)
		{
			this->_response.setFd(this->_fd);
			throw (ResponseException(408));
		}
		if (this->_response_flag == false)
		throw (ResponseException(500));
		if (this->_request.getStatusNumber() != 0)
		{
			throw (ResponseException(this->_request.getStatusNumber()));
		}
		if (this->_request.getConnectionFlag() == true)
		{
			if (epoll_ctl(epoll_fd, EPOLL_CTL_DEL,this->_fd, NULL) == -1) {
				throw (ResponseException(500));
			}
		}
		else if (epoll_ctl(epoll_fd, EPOLL_CTL_MOD,this->_fd, &ev) == -1) {
			throw (ResponseException(500));
		}
		this->_response.ExecuteResponse(this->_request);
		if (this->_request.getConnectionFlag() == true)
		{
			closeAndDeleteSocket(socket);
			return ;
		}
	}
	catch (const ResponseException& e)
	{
		this->_response.setStatusCode(e.getStatus());
		if (epoll_ctl(epoll_fd, EPOLL_CTL_DEL,this->_fd, NULL) == -1) {
				this->_response.setStatusCode(500);
			}
		if ((this->_response.getRedirectUri()).empty())
			checkErrorPages(this->_response.getStatusCode());
		this->_response.closeResponse(this->_error_file_flag);
		closeAndDeleteSocket(socket);
		return ;
	}
	catch (std::exception& e)
	{
		if (epoll_ctl(epoll_fd, EPOLL_CTL_DEL,this->_fd, NULL) == -1) {
				this->_response.setStatusCode(500);
			}
		this->_response.setStatusCode(500);
		checkErrorPages(500);
		this->_response.closeResponse(this->_error_file_flag);
		closeAndDeleteSocket(socket);
		return ;
	}
	this->_request.reSetRequest();
	this->_response.reSetResponse();
	this->reSetClientSocket();
	return ;
}

bool ClientSocket::handleTimeOut(int epoll_fd, std::map<int, ASocket*>& _socket, int fd) 
{
		struct epoll_event ev;
	ev.events = EPOLLOUT;
		ev.data.fd = fd;

	(void)_socket;
	(void)fd;
	if (this->getTimeOut() == true)	
		return (true);
	if (epoll_ctl(epoll_fd, EPOLL_CTL_MOD, fd, &ev) == -1) {
			this->setTimeOut(true);
		}
	this->setTimeOut(true);
	return (true);
}
