#pragma once

#include <string>
#include <iostream>
#include "Request.hpp"
#include "Response.hpp"

#define BUFFER_SIZE 200

class Client
{
	// private:

	public:
		int _fd;
		std::string _buffer;
		// std::vector<LocationConfig> _location;
		Request _request;
		Response _response;
		bool _buffer_flag;

		Client();
		bool CheckCarrigereturn(std::string& buffer);
		void RestOfBuffer(std::string& buffer, std::string::iterator& it);
		bool LoopRequest(std::string& buffer);
		bool AcceptRequest();

};

bool SubstringObject(std::string& buffer, std::string::iterator& it,std::string& object);