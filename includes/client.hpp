#pragma once

#include <string>
#include <iostream>
#include <cstdlib>
#include "Request.hpp"
#include "Response.hpp"
#include "location.hpp"
#include "nginx.hpp"


#define BUFFER_SIZE 200

class Client
{
	// private:

	public:
		int _fd;

		std::string _buffer;
		ServerConfig _conf;
		Request _request;
		Response _response;
		bool _first_post_flag;
		bool _complete_post_flag;
		bool _complete_parse_flag;
		bool _post_body_flag;

		Client();
		Client(ServerConfig conf);
		bool CheckCarrigereturn(std::string& buffer);
		void RestOfBuffer(std::string& buffer, std::string::iterator& it);
		// bool LoopRequest(std::string& buffer);
		bool AcceptRequest();
		bool ValidRequest(std::string& buffer, std::string::iterator& it, std::string& object);
		bool CheckCRequestFlag(std::string& buffer, std::string::iterator& it, std::string& object);
		bool CheckPostFlag();
		bool CloseClientFd();
		bool CheckExecuteResponse(void);
		void ValidLocation(LocationConfig& location, LocationConfig& location_tmp, bool& location_flag);
		bool CheckAndChangeLocationUri(std::vector<LocationConfig>& location, const std::string& uri);
		void CombineUriAndLocationRoot(LocationConfig& location);
		void ChangeDefaultPath(const std::string& uri);
		bool CheckAndChangeRootUri(const std::string& uri);
		void ChangeConfUri(const std::string& uri);

};

bool SubstringObject(std::string& buffer, std::string::iterator& it,std::string& object);