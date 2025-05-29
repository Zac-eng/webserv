#include <iostream>
#include "ErrorPage.hpp"

int main() {
	ErrorPage errorPage;
	int codes[] = {400, 404, 405, 413, 500, 504, 505};
	for (int i = 0; i < 7; ++i) {
		int code = codes[i];
		const ErrorPage::Page& page = errorPage.get(code);
		std::cout << "Status Code: " << code << std::endl;
		std::cout << " Path: " << page.path << std::endl;
		std::cout << " Media Type: " << page.media_type << std::endl;
		// int client_fd = ;
		std::string response = errorPage.createErrorResponse(code);
		std::cout << "response: " << response << std::endl;
		// send(client_fd, response.c_str(), response.size(), 0);
		std::cout << std::endl;
	}
	return 0;
}