#pragma once

#include <exception>
#include <string>


class RequestException : public std::exception
{
	private:
		size_t _status;
		std::string _message;

	public:
		RequestException(size_t status, const std::string& message);
		const char* what() const throw();
		virtual ~RequestException() throw();
		int getStatus(void) const;
};
