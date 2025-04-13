#include "ListenSocket.hpp"
#include "ClientSocket.hpp"
#include "Server.hpp"

ServerConfig ListenSocket::GetConf()
{
	return (this->_conf);
}


ListenSocket::ListenSocket() {}

ListenSocket::ListenSocket(ServerConfig& conf) : ASocket(conf) {};

ListenSocket::~ListenSocket() {}

bool ListenSocket::SocketInit(void)
{
	this->_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (this->_fd < 0)
		throw std::runtime_error("Socket Create");
	return (true);
}

bool ListenSocket::SetSocket(void)
{
	int sockopt = 0;
	int optval = 1;

	sockopt = setsockopt(this->_fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(int));
	// 第４引数では、型が色々あるため、先頭アドレスを渡して、第五引数で、正しく型を戻している。（構造体とか）
	if (sockopt < 0)
		throw std::runtime_error("Set Socket");
	return (true);
}

bool ListenSocket::BindSocket()
{
	struct sockaddr_in address;
	unsigned long ip_address;

	memset(&address, 0, sizeof(address));
	address.sin_family = AF_INET;
	address.sin_port = htons(this->_conf.listen_port);
	ip_address = inet_addr(this->_conf.server_name.c_str());
	if (ip_address == INADDR_NONE)
		throw std::runtime_error("IP Address");
	address.sin_addr.s_addr = ip_address;

	if (bind(this->_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
		throw std::runtime_error("Bind Address");
	this->_host_name = this->_conf.server_name;
	this->_port = this->_conf.listen_port;
	return (true);
}

bool ListenSocket::CreateListenSocket()
{
	if (listen(this->_fd, 3) < 0)
		throw std::runtime_error("Listen Socket");
	return (true);
}

bool ListenSocket::CreateSocket(void)
{
	if (SocketInit() == false)
		return (false);
	if (SetSocket() == false)
		return (false);
	if (BindSocket() == false)
		return (false);
	if (CreateListenSocket() == false)
		return (false);
	// this->_listen_fd = true;
	return (true);
}

bool ListenSocket::HandleEpollInEvent(int epoll_fd, std::map<int, ASocket*>& socket)
{
	int fd;
	ASocket *client = new ClientSocket(this->_conf);
	struct sockaddr_in address;
	socklen_t len = sizeof(address);
	struct epoll_event event;

	memset(&address, 0 ,len);
	fd = accept(this->_fd, (struct sockaddr *)&address, &len);
	if (fd < 0)
		return (false);
	event.events = EPOLLIN;
	event.data.fd = fd;
	if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fd, &event) < 0)
		throw std::runtime_error("epoll_ctl");
	client->SetFd(fd);
	socket.insert(std::make_pair(client->GetFd(), client));
	if (SetNonBlocking(client->GetFd()) == false)
		return (false);
	client->setClinetAddr(address);
	return(true);
}

void ListenSocket::HandleEpollOutEvent()
{
	return ;
}