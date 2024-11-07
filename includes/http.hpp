#pragma once

#include <iostream>

class HttpRequest {
	private:
		std::vector<std::string> _http_request;

	public:
		HttpRequest();
		~HttpRequest();
}