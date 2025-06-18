#pragma once

#include <exception>
#include <string>


class ServerException : public std::exception
{
	private:
		std::string _message;
	
	public:
		ServerException();
		ServerException(const std::string& message);
		const char* what() const throw();

		virtual ~ServerException() throw();

};
