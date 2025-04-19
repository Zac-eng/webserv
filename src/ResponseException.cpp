#include "ResponseException.hpp"

ResponseException::ResponseException(size_t status) : _status(status) {}

const char* ResponseException::what() const throw()
{
	return ("status code error");
}

int ResponseException::getStatus(void) const
{
	return (this->_status);
}
