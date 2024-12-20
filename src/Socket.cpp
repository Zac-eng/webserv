#include "Socket.hpp"

Socket::Socket() {};

Socket::~Socket() {};


Socket::Socket(ServerConfig& conf) : _conf(conf) {};


bool Socket::SocketInit(void)
{
	this->_server_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (this->_server_fd < 0)
		return (false);
	return (true);
}

bool Socket::SetSocket(void)
{
	int sockopt = 0;
	int optval = 1;

	sockopt = setsockopt(this->_server_fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(int));
	// 第４引数では、型が色々あるため、先頭アドレスを渡して、第五引数で、正しく型を戻している。（構造体とか）
	if (sockopt < 0)
		return (false);
	return (true);
}

bool Socket::BindSocket()
{
	struct sockaddr_in address;

	memset(&address, 0, sizeof(address));
	address.sin_family = AF_INET;
	address.sin_port = htons(8080);
	address.sin_addr.s_addr = inet_addr("127.0.0.1");

	if (bind(this->_server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
		return (false);
	return (true);
}

bool Socket::ListenSocket()
{
	if (listen(this->_server_fd, 3) < 0)
		return (false);
	return (true);
}

bool Socket::SocketCreate(void)
{
	if (SocketInit() == false)
		return (false);
	if (SetSocket() == false);
		return (false);
	if (BindSocket() == false)
		return (false);
	if (ListenSocket() == false)
		return (false);
	// this->_listen_fd = true;
	return (true);
}
