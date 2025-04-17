#include "RequestException.hpp"

RequestException::RequestException(size_t status) : _status(status) {}

const char* RequestException::what() const throw()
{
	return ("status code error");
}

int RequestException::getStatus(void) const
{
	return (this->_status);
}
