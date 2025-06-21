#include "ListenSocket.hpp"
#include "ClientSocket.hpp"
#include "Server.hpp"

ServerConfig ListenSocket::getConf() const
{
	return (this->_conf);
}


ListenSocket::ListenSocket() {}

ListenSocket::ListenSocket(ServerConfig& conf) : ASocket(conf) {};

ListenSocket::~ListenSocket() {}

bool ListenSocket::socketInit(void)
{
	this->_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (this->_fd < 0)
		return (false);
	return (true);
}

bool ListenSocket::setSocket(void)
{
	int sockopt = 0;
	int optval = 1;

	sockopt = setsockopt(this->_fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(int));
	// 第４引数では、型が色々あるため、先頭アドレスを渡して、第五引数で、正しく型を戻している。（構造体とか）
	if (sockopt < 0)
		return (false);
	return (true);
}

bool ListenSocket::bindSocket()
{
	struct sockaddr_in address;
	unsigned long ip_address;

	memset(&address, 0, sizeof(address));
	address.sin_family = AF_INET;
	address.sin_port = htons(this->_conf.getListenPort());
	ip_address = inet_addr((this->_conf.getServerName()).c_str());
	std::cout << this->_conf.getServerName()<<std::endl;
	if (ip_address == INADDR_NONE)
	{
		return (false);
	}
	address.sin_addr.s_addr = ip_address;
	if (bind(this->_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
	{
			std::cout << this->_fd<< std::endl;
		return (false);
	}
	this->_host_name = this->_conf.getServerName();
	this->_port = this->_conf.getListenPort();
	return (true);
}

bool ListenSocket::createListenSocket()
{
	if (listen(this->_fd, 3) < 0)
		return (false);
	return (true);
}

bool ListenSocket::createSocket(void)
{
	if (socketInit() == false)
		return (false);
	if (setSocket() == false)
		return (false);
	if (bindSocket() == false)
		return (false);
	if (createListenSocket() == false)
		return (false);

	// this->_listen_fd = true;
	return (true);
}

void ListenSocket::handleEpollInEvent(int epoll_fd, std::map<int, ASocket*>& socket)
{
	int fd;
	struct sockaddr_in address;
	socklen_t len = sizeof(address);
	struct epoll_event event;
	time_t start_time;

	start_time = 0;
	memset(&address, 0 ,len);
	fd = accept(this->_fd, (struct sockaddr *)&address, &len);
	if (fd < 0)
		return ;
	ASocket *client = new ClientSocket(this->_conf, address);
	event.events = EPOLLIN;
	event.data.fd = fd;
	if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fd, &event) < 0)
	{
		delete (client);
		return ;
	}
	client->setFd(fd);
	socket.insert(std::make_pair(client->getFd(), client));
	if (set_nonblocking(client->getFd()) == false)
	{
		if (epoll_ctl(epoll_fd, EPOLL_CTL_DEL, fd, &event) < 0)
	{
		delete (client);
		socket.erase(client->getFd());

		return ;
	}
		delete (client);
		socket.erase(client->getFd());
		return ;
	}
	start_time = time(NULL);
	client->setStartTime(start_time);
	return;
}

void ListenSocket::handleEpollOutEvent(int epoll_fd, std::map<int, ASocket*>& _socket)
{
	(void)epoll_fd;
	(void)_socket;
	return ;
}

bool ListenSocket::handleTimeOut(int epoll_fd, std::map<int, ASocket*>& _socket, int fd) 
{

	(void)epoll_fd;
	(void)_socket;
	(void)fd;
	return (true);
}
