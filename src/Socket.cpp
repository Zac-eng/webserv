#include "Socket.hpp"

Socket::Socket() : _listen_flag(true) {};

Socket::~Socket() {};

Socket::Socket(bool listen_flag) : _listen_flag(listen_flag) {};

Socket::Socket(ServerConfig& conf) : _conf(conf), _listen_flag(true) {};

ServerConfig Socket::GetConf()
{
	return (this->_conf);
}

Socket::Socket(const Socket& other)
{
	this->_listen_flag = other._listen_flag;
	this->_server_fd = other._server_fd;
	this->_port = other._port;
}

bool Socket::SocketInit(void)
{
	this->_server_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (this->_server_fd < 0)
		throw std::runtime_error("Socket Create");
	return (true);
}

bool Socket::SetSocket(void)
{
	int sockopt = 0;
	int optval = 1;

	sockopt = setsockopt(this->_server_fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(int));
	// 第４引数では、型が色々あるため、先頭アドレスを渡して、第五引数で、正しく型を戻している。（構造体とか）
	if (sockopt < 0)
		throw std::runtime_error("Set Socket");
	return (true);
}

bool Socket::BindSocket()
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

	if (bind(this->_server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
		throw std::runtime_error("Bind Address");
	this->_host_name = this->_conf.server_name;
	this->_port = this->_conf.listen_port;
	return (true);
}

bool Socket::ListenSocket()
{
	if (listen(this->_server_fd, 3) < 0)
		throw std::runtime_error("Listen Socket");
	return (true);
}

bool Socket::SocketCreate(void)
{
	if (SocketInit() == false)
		return (false);
	if (SetSocket() == false)
		return (false);
	if (BindSocket() == false)
		return (false);
	if (ListenSocket() == false)
		return (false);
	// this->_listen_fd = true;
	return (true);
}
