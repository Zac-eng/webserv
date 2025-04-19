#pragma once

#include <exception>
#include <string>


class ServerException : public std::exception
{
	public:
		ServerException();
		const char* what() const throw();
};
