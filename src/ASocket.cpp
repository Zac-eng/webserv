#include "ASocket.hpp"

ASocket::ASocket() {};

ASocket::~ASocket() {};

int ASocket::GetFd(void) const
{
	return (this->fd);
}

void ASocket::SetFd(int fd)
{
	this->_fd = fd;
}