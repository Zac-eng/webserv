#include "Socket.hpp"

Socket::Socket() : _listen_flag(true) {};

Socket::~Socket() {};

Socket::Socket(bool listen_flag) : _listen_flag(listen_flag) {};

Socket::Socket(ServerConfig& conf) : _conf(conf), _listen_flag(true) {};

Socket::Socket(const Socket& other)
{
	this->_listen_flag = other._listen_flag;
	this->_server_fd = other._server_fd;
	this->_port = other._port;


}

void debug()
{
	std::cout << "デバック: ----------"<< std::endl;
	exit(0);
}

void debugconf(ServerConfig& conf)
{
	std::cout << conf._listen_port << ": " << conf._server_name << std::endl;
	exit(0);
}


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
	unsigned long ip_address;

	memset(&address, 0, sizeof(address));
	address.sin_family = AF_INET;
	address.sin_port = htons(this->_conf._listen_port);
	ip_address = inet_addr(this->_conf._server_name.c_str());
	if (ip_address == INADDR_NONE)
		return (false);
	address.sin_addr.s_addr = ip_address;

	if (bind(this->_server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
	{
		std::cout << "error" <<std::endl;
		return (false);
	}
	this->_host_name = this->_conf._server_name;
	this->_port = this->_conf._listen_port;
	return (true);
}

bool Socket::ListenSocket()
{
	if (listen(this->_server_fd, 3) < 0)
	{
		std::cout << "error" <<std::endl;
		return (false);
	}
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

void debugSocket(Socket& socket)
{
	std::cout << socket._conf._listen_port << socket._conf._server_name<<std::endl;
	std::cout << socket._server_fd<<": "<<socket._port<<": "<<socket._host_name<<std::endl;
	exit(1);
}