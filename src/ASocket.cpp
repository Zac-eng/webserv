#include "ASocket.hpp"

ASocket::ASocket() {};

ASocket::~ASocket() {};

ASocket::ASocket(ServerConfig& conf) : _conf(conf) {};

int ASocket::getFd(void) const
{
	return (this->_fd);
}

void ASocket::setFd(int fd)
{
	this->_fd = fd;
}

ServerConfig ASocket::getConf(void) const
{
	return (this->_conf);
}

void ASocket::setConf(ServerConfig conf)
{
	this->_conf = conf;
}

std::string ASocket::getHostName(void) const
{
	return (this->_host_name);
}

void ASocket::setHostName(std::string& host_name)
{
	this->_host_name = host_name;
}

int ASocket::getPort(void) const
{
	return (this->_port);
}

void ASocket::setPort(int port)
{
	this->_port = port;
}
