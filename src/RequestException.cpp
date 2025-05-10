#include "RequestException.hpp"

RequestException::RequestException(size_t status, const std::string& message) : _status(status), _message(message) {}

const char* RequestException::what() const throw()
{
	return (this->_message.c_str());
}

size_t RequestException::getStatus(void) const
{
	return (this->_status);
}

RequestException::~RequestException() throw() {}