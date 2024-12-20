#pragma once

#define BUFFER_SIZE 200;

class Client
{
	private:
		int _fd;
		std::string _buffer;
		std::vector<LocationConfig> _location;
		Request _request;
		Response _response;
		bool _buffer_flag;

	public:
		Client();

}