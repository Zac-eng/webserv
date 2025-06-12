#include "ASocket.hpp"

ASocket::ASocket() : _time_out_flag(false) {};

ASocket::~ASocket() {};

ASocket::ASocket(ServerConfig& conf) : _conf(conf), _start_time(-1), _time_out_flag(false) {};

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

void ASocket::setStartTime(time_t start_time)
{
	this->_start_time = start_time;
}

time_t ASocket::getStartTime(void)
{
	return (this->_start_time);
}

bool ASocket::getTimeOut(void) const
{
	return (this->_time_out_flag);
}


void ASocket::setTimeOut(bool time_out_flag)
{
	this->_time_out_flag = time_out_flag;
}