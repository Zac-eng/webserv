#pragma once

#include <exception>
#include <string>


class ResponseException : public std::exception
{
	private:
		size_t _status;

	public:
		ResponseException(size_t status);
		const char* what() const throw();
		size_t getStatus(void) const;
};
