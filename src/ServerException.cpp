#include "ServerException.hpp"

ServerException::ServerException() {};

ServerException::ServerException(const std::string& message) : _message(message) {}

const char* ServerException::what() const throw()
{
	return (this->_message.c_str());
}

ServerException::~ServerException() throw() {}