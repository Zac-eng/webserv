#pragma once

#include <string>
#include <iostream>
#include "Request.hpp"
#include "Response.hpp"

#define BUFFER_SIZE 200

class Client
{
	// private:
		int _fd;
		std::string _buffer;
		// std::vector<LocationConfig> _location;
		Request _request;
		Response _response;
		bool _buffer_flag;

	public:
		Client();
		bool CheckCarrigereturn(std::string& buffer);
		void RestOfBuffer(std::string& buffer, std::string::iterator& it);
		void LoopRequest(std::string& buffer);
		bool AcceptRequest();

};
std::string SubstringObject(const std::string& buffer, std::string::iterator& it);