#pragma once

#include <exception>
#include <string>


class RequestException : public std::exception
{
	private:
		size_t _status;

	public:
		RequestException(size_t status);
		const char* what() const throw();
		int getStatus(void) const;
};
