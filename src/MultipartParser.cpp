#include "MultipartParser.hpp"

MultipartParser::MultipartParser() : _start_flag(false), _end_flag(false) {};


void MultipartParser::executeParse(const std::string& request)
{
	std::string object;

	if (this->_start_flag == false)
	{
		if (request == this->_boundary)
		{
			this->_start_flag = true;
			return ;
		}
		else
			throw RequestException(400, "error");
	}
	if (request == this->_boundary)

	this->_body += request;
	

}