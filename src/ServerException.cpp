#include "ServerException.hpp"

ServerException::ServerException() {};

const char* ServerException::what() const throw()
{
	return ("Error Server Exception");
}