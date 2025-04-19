#include "ASocket.hpp"

ASocket::ASocket() {};

ASocket::~ASocket() {};

ASocket::ASocket(ServerConfig& conf) : _conf(conf) {};

int ASocket::GetFd(void) const
{
	return (this->_fd);
}

void ASocket::SetFd(int fd)
{
	this->_fd = fd;
}
