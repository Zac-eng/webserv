#include "ResponseException.hpp"

ResponseException::ResponseException(size_t status) : _status(status) {}

const char* ResponseException::what() const throw()
{
	return ("status code error");
}

size_t ResponseException::getStatus(void) const
{
	return (this->_status);
}

void ResponseException::setStatus(size_t status)
{
	this->_status = status;
	return ;
}
